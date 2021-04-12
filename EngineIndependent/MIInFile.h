#pragma once
#include "UniquePtr.h"
#include "FileMode.h"

class FILE;

namespace MI {
	class String;
	class Buffer;
	class FMode;
	//File class - might be obsolete with MMapFile class, + something else for stdin and stdout, 
	//		however it might have the advantage of being a bit more lightweight, idk
	struct InFile {
		//Specify as | product of flags, Ex.  File::Mode::IN | File::Mode::OUT | File::Mode::BIN opens for binary input + output

		//setup buffer, and attach it to a file stream (if one is given to constructor)
		InFile(const int bufcap = 16384);
		InFile(const char* filename, const FMode& mode = inmode, const int bufcap = 16384);
		InFile(const FILE* cfile, const FMode& mode = inmode, const int bufcap = 16384);
		InFile(InFile&& other); //move the resource from other file to this one
		~InFile();

		//opens a new file with the specified mode, releasing previous file if necessary
		void Open(const char* filename, const FMode& mode);
		//sets up file buffer
		void Close();

		bool Read(Buffer& mbuff, size_t readcap);
		bool Read(String& strbuff);
		bool ReadLine(Buffer& mbuff, size_t readcap);
		bool ReadLine(String& strbuff);
	private: //pImpl
		struct Impl;
		UniquePtr<Impl> pImpl;
	};
}