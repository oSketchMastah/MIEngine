#include "MIFile.h"
#include "MIFileMode.h"
#include "MIBuffer.h"
#include "MIAssert.h"
#include "MIString.h"
#include "MIStringUtils.h"
#include "MIUtilities.h"
#include <cstdio>

using namespace MI;
struct File::Impl {
	Impl() : mode{ DefaultFileMode }, m_pFile{ nullptr }, buffCap{0}, buff() {}
	//Must specify a capacity for buffer, whether implicit or whatever
	Impl(const int bufcap) : mode{ DefaultFileMode }, m_pFile{ nullptr }, buffCap{ bufcap }, buff(buffCap) {}
	~Impl() { fclose(m_pFile); m_pFile = 0; }
	Impl(FILE* pFile, const int bufcap) : mode{ DefaultFileMode }, m_pFile{ pFile }, buffCap{ bufcap }, buff(buffCap) //this is here so you can get a file object from stdout or stdin
	{
		if (pFile == stdin) { mode = FMode::in; }
		else if (pFile == stdout) { mode = FMode::out; } //could also do stderr, but nah
		else { DoAssertMsg("File construction from unsupported FILE*, only stdin and stdout work right now"); }
	};

	FMode mode;
	::FILE* m_pFile;
	int buffCap; //TODO: use this
	Buffer buff;

	void Open(const char* filename, const FMode& openmode = DefaultFileMode) {
		FMode mode(openmode);
		m_pFile = fopen(filename, mode);
	}

	void AttachBufferToStream() {
		Assert(setvbuf(m_pFile, buff.RawMemory(), _IOFBF, buffCap) == 0); //IOFBF = full buffering
	}

	bool NeedsFlushToAdd(size_t addedLength) {
		return buffCap >= buff.Size() + addedLength;
	}

	void FlushOutput() {
		fflush(m_pFile);
		buff.SetSize(0);
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
{}

File::File(const char* filename, const FMode& mode, const int bufcap) : pImpl{ new Impl(bufcap) }
{
	pImpl->Open(filename, mode);
	pImpl->AttachBufferToStream();
}

File::File(FILE* cfile, const int bufcap) : pImpl{ new Impl(cfile, bufcap) } {}

File::File(File&& other) {
	Swap(pImpl, other.pImpl);
}

File::~File() {
	if (pImpl->m_pFile) { fclose(pImpl->m_pFile); }
}

//opens a new file with the specified mode, releasing previous file if necessary
void File::Open(const char* filename, const FMode& newmode) { //buflen
	if (pImpl->m_pFile) { fclose(pImpl->m_pFile); } //close the current file if necessary
	pImpl->Open(filename, newmode);
	pImpl->AttachBufferToStream();
}

void File::Close() {
	fclose(pImpl->m_pFile);
	pImpl->m_pFile = nullptr;
}

void File::Write(const Buffer& data) {
	if (pImpl->NeedsFlushToAdd(data.Size())) {
		pImpl->FlushOutput(); //flushes our buffer to the file
	}
	pImpl->buff.AddEnd(data.c_str(), data.Size());
}

void File::WriteLine(const Buffer& data, const char *lineEnd) {
	const int endlen = MI::strlen(lineEnd);
	if (pImpl->NeedsFlushToAdd(data.Size() + endlen)) {
		pImpl->FlushOutput(); //flushes our buffer to the file
	}
	pImpl->buff.AddEnd(data.c_str(), data.Size());
	pImpl->buff.AddEnd(lineEnd, endlen);
}
void File::Write(const char* data, const size_t size) {
	if (pImpl->NeedsFlushToAdd(size)) {
		pImpl->FlushOutput(); //flushes our buffer to the file
	}
	pImpl->buff.AddEnd(data, size);
}

bool File::Read(Buffer& mbuff, size_t readcap) {
	return pImpl->Read(mbuff, readcap);
}

bool File::Read(String& str) {
	return pImpl->Read(str.GetBuffer(), str.Capacity());
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