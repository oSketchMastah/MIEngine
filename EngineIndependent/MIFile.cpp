#include "MIFile.h"
#include "MIFileMode.h"
#include "MIBuffer.h"
#include "MIAssert.h"
#include "MIString.h"
#include "MIStringUtils.h"
#include "MIUtilities.h"
#include <cstdio>

#pragma warning(disable : 4996) //doing this since fopen is portable and can be used right

constexpr size_t internal_buffer_cap = 16384; //some windows OS limit the internal buffer size to 16384 bytes precisely, and is all-around pretty optimal

using namespace MI;
struct File::Impl {

	Impl() : mode{ DefaultFileMode }, m_pFile{ nullptr }, __internalbufCap{ internal_buffer_cap }, __internalbuf(internal_buffer_cap),
		__programbufCap(0), __programbuf() {}

	//Must specify a capacity for buffer, whether implicit or whatever
	Impl(const int progbufcap) : mode{ DefaultFileMode }, m_pFile{ nullptr }, __internalbufCap{ internal_buffer_cap }, __internalbuf(internal_buffer_cap), 
		__programbufCap{ progbufcap }, __programbuf(__programbufCap) {}


	~Impl() { if (m_pFile) { fclose(m_pFile); } }


	Impl(const String& filename, const int bufcap) : mode{ DefaultFileMode }, m_pFile{ fopen(filename, mode) }, __internalbufCap{ internal_buffer_cap }, __internalbuf(internal_buffer_cap), 
		m_filename { filename }, __programbufCap{ bufcap }, __programbuf(__programbufCap) //this is here so you can get a file object from stdout or stdin
	{

	};


	FMode mode;
	::FILE* m_pFile;
	String m_filename;

	int __internalbufCap; //TODO: use this
	//partially given to the std lib to privately buffer as needed for IO ops, we still must update size data
	Buffer __internalbuf;

	//A buffer which you can manipulate as you please, you can steal this, move anew buffer in, whatever.
	//using this as a layer before __internalbuf VIA fwrite/fread allows for more control over how output happens
	//
	int __programbufCap;
	Buffer __programbuf;

	void Open(const char* filename, const FMode& openmode = DefaultFileMode) {
		m_filename = filename;
		m_pFile = fopen(filename, FMode(openmode));
		if (m_pFile == nullptr && openmode[0] == DefaultFileMode[0] && openmode[1] == DefaultFileMode[1]) { //(r+ does NOT open a nonexistent file)
			m_pFile = fopen(filename, FMode(FMode::out | FMode::in | FMode::ovw)); //fix for a common error, forgetting to specify ovw flag
		}
	}

	void AttachInternalBufferToStream() {
		if (m_pFile) {//buff.RawMemory()
			Assert(setvbuf(m_pFile, __internalbuf.RawMemory(), _IOFBF, __internalbufCap - 1) == 0); //IOFBF = full buffering
		}
	}

	bool NeedsFlushToAdd(size_t addedLength) {
		return __programbuf < __programbuf.Size() + addedLength;
	}

	void FlushOutput() {
		m_pFile = fopen(m_filename, mode); //incase inactivity causes it to close automatically
		fwrite(__programbuf.RawMemory(), 1, __programbuf.size + 1, m_pFile); //transfer program buffer to internal
		fflush(m_pFile); //flush internal buffer to character device
		__internalbuf.SetSize(0); //update the size of internal Buffer ourselves
	}

	//true if you need to keep reading to get more data
	bool Read(Buffer& mbuff, size_t readcap) {
		mbuff.SetSize(fread(mbuff.RawMemory(), 1, readcap, m_pFile));
		return mbuff.Size() == readcap;
	}

	bool ReadChar(char* rawmem) {
		return fread(rawmem, 1, 1, m_pFile) == 1;
	}
};

//******************************************* FILE ***************************************************

	//Specify as | product of flags, Ex.  File::Mode::IN | File::Mode::OUT | File::Mode::BIN opens for binary input + output

//construct and set file for IO  /* mode == Mode::IN | Mode::BIN  for binary reading */
File::File(const int bufcap) : pImpl{ new Impl(bufcap) }
{

}

File::File(const char* filename, const FMode& mode, const int bufcap) : pImpl{ new Impl(bufcap) }
{
	pImpl->Open(filename, mode);
	pImpl->AttachInternalBufferToStream();
}

File::File(const String& cfile, const int bufcap) : pImpl{ new Impl(cfile, bufcap) } {}

File::File(File&& other) {
	Swap(pImpl, other.pImpl);
}

File::~File() {
	//if (pImpl->m_pFile) { fclose(pImpl->m_pFile); }
}

//opens a new file with the specified mode, releasing previous file if necessary
void File::Open(const char* filename, const FMode& newmode) { //buflen
	pImpl->m_filename = filename;
	if (pImpl->m_pFile) { 
		fclose(pImpl->m_pFile); 
	} //close the current file if necessary
	pImpl->Open(filename, newmode);
	pImpl->AttachInternalBufferToStream();
}

void File::Close(bool outremaining) {
	fwrite(pImpl->__programbuf, 1, pImpl->__programbuf.size, pImpl->m_pFile);
	fflush(pImpl->m_pFile);
	fclose(pImpl->m_pFile);
}

void File::Write(const Buffer& data) {
	if (pImpl->NeedsFlushToAdd(data.Size())) {
		pImpl->FlushOutput(); //flushes our buffer to the file
	}
	pImpl->__internalbuf.AddEnd(data.c_str(), data.Size());
}

void File::WriteLine(const Buffer& data, const char *lineEnd) {
	const int endlen = MI::strlen(lineEnd);
	if (pImpl->NeedsFlushToAdd(data.Size() + endlen)) {
		pImpl->FlushOutput(); //flushes our buffer to the file
	}
	pImpl->__programbuf.AddEnd(data.c_str(), data.Size());
	pImpl->__programbuf.AddEnd(lineEnd, endlen);
}
void File::WriteLine(const char* data, const char* lineEnd) {
	const int len = MI::strlen(data);
	const int endlen = MI::strlen(lineEnd);
	if (pImpl->NeedsFlushToAdd(len + endlen)) {
		pImpl->FlushOutput(); //flushes our buffer to the file
	}
	//printf("%s%d%s", pImpl->__internalbuf.c_str(), pImpl->__internalbuf.size, "\n");
	pImpl->__programbuf.AddEnd(data, len);
	pImpl->__programbuf.AddEnd(lineEnd, endlen);
	//printf("%s%d%s", pImpl->__internalbuf.c_str(), pImpl->__internalbuf.size, "\n");
	///printf("%s%d%s", pImpl->__internalbuf.c_str(), pImpl->__internalbuf.size, "\n");
}

void File::Write(const char* data, const size_t size) {
	if (pImpl->NeedsFlushToAdd(size)) {
		pImpl->FlushOutput(); //flushes our buffer to the file
	}
	pImpl->__programbuf.AddEnd(data, size);
}

bool File::Read(Buffer& mbuff, size_t readcap) {
	return pImpl->Read(mbuff, readcap);
}

bool File::Read(String& str) {
	return pImpl->Read(str.GetBuffer(), str.Capacity());
}

size_t File::ProgramBufferCapacity() const {
	return pImpl->__programbufCap;
}

Buffer& File::Read() {
	pImpl->Read(pImpl->__programbuf, pImpl->__programbufCap);
	return pImpl->__programbuf;
}

bool File::ReadLine(Buffer& mbuff, const size_t buffcap) {
	char* mem = mbuff.RawMemory();
	size_t totalread = 0;
	for (; totalread < buffcap - 1 && pImpl->ReadChar(mem); totalread++, mem++) {
		if (*mem == '\n') { break; }
	}
	mem++; *mem = '\0';
	mbuff.SetSize(totalread);

	AssertMsg(totalread != buffcap - 1, "buffer overflow");
	return totalread > 0;
}

bool File::ReadLine(String& str) {
	return ReadLine(str.GetBuffer(), str.Capacity());

}

Buffer& File::ReadLine(bool & tocontinue) {
	tocontinue = ReadLine(pImpl->__programbuf, pImpl->__programbufCap);
	return pImpl->__programbuf;
}

