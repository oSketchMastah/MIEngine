#include "OutFile.h"
#include "Buffer.h"
#include "Assert.h"
#include "String.h"
#include "EngineStringUtils.h"
#include "Swap.h"
#include "FileMode.h"
#include <cstdio>

using namespace MI;

struct OutFile::Impl {
	//Must specify a capacity for buffer, whether implicit or whatever
	Impl(const int bufcap) : mode{ DefaultFileMode }, m_pFile { nullptr }, buffCap{ bufcap }, buff(buffCap) {}
	~Impl() { fclose(m_pFile); m_pFile = 0; }
	Impl(FILE* pFile, const int bufcap) : mode{ DefaultFileMode }, m_pFile { pFile }, buffCap{ bufcap }, buff(buffCap) //this is here so you can get a file object from stdout or stdin
	{
		DoAssertMsg("File construction from unsupported FILE*, only stdin and stdout work right now");
	};

	FMode mode;
	::FILE* m_pFile;
	int buffCap; //TODO: use this
	Buffer buff;

	void Open(const char* filename, const FMode& openmode = DefaultFileMode) {
		FMode mode (openmode);
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
OutFile::OutFile(const int bufcap) : pImpl{ new Impl(bufcap) }
{}

OutFile::OutFile(const char* filename, const FMode& mode, const int bufcap) : pImpl{ new Impl(bufcap) }
{
	pImpl->Open(filename, mode);
	pImpl->AttachBufferToStream();
}

OutFile::OutFile(FILE* cfile, const int bufcap) : pImpl{ new Impl(cfile, bufcap) } {}

OutFile::OutFile(OutFile&& other) {
	Swap(pImpl, other.pImpl);
}

OutFile::~OutFile() {
	if (pImpl->m_pFile) { fclose(pImpl->m_pFile); }
}

//opens a new file with the specified mode, releasing previous file if necessary
void OutFile::Open(const char* filename, const FMode& newmode) { //buflen
	if (pImpl->m_pFile) { fclose(pImpl->m_pFile); } //close the current file if necessary
	pImpl->Open(filename, newmode);
	pImpl->AttachBufferToStream();
}

void OutFile::Close() {
	fclose(pImpl->m_pFile);
	pImpl->m_pFile = nullptr;
}

void OutFile::Write(const Buffer& data) {
	if (pImpl->NeedsFlushToAdd(data.Size())) {
		pImpl->FlushOutput(); //flushes our buffer to the file
	}
	pImpl->buff.AddEnd(data.c_str(), data.Size());
}

void OutFile::WriteLine(const Buffer& data, const char* lineEnd) {
	const int endlen = MI::strlen(lineEnd);
	if (pImpl->NeedsFlushToAdd(data.Size() + endlen)) {
		pImpl->FlushOutput(); //flushes our buffer to the file
	}
	pImpl->buff.AddEnd(data.c_str(), data.Size());
	pImpl->buff.AddEnd(lineEnd, endlen);
}
void OutFile::Write(const char* data, const size_t size) {
	if (pImpl->NeedsFlushToAdd(size)) {
		pImpl->FlushOutput(); //flushes our buffer to the file
	}
	pImpl->buff.AddEnd(data, size);
}
