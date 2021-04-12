#pragma once
#include "MIUniquePtr.h"
#include <cstdio>
//struct FILE;
namespace MI {
	class String;
	class Buffer;
	class FMode;
	//File class - might be obsolete with MMapFile class, + something else for stdin and stdout, 
	//		however it might have the advantage of being a bit more lightweight, idk
	struct File {
		//Specify as | product of flags, Ex.  File::Mode::IN | File::Mode::OUT | File::Mode::BIN opens for binary input + output

		//setup buffer, and attach it to a file stream (if one is given to constructor)
		File(const int bufcap = 16384);
		File(const char* filename, const FMode& mode, const int bufcap = 16384);
		File(FILE* cfile, const int bufcap = 16384);
		File(File&& other); //move the resource from other file to this one
		~File();

		//opens a new file with the specified mode, releasing previous file if necessary
		void Open(const char* filename, const FMode& mode);
		//sets up file buffer
		void Close();

		void Write(const Buffer& data);
		void WriteLine(const Buffer& data, const char* lineEnd = "\n");
		void Write(const char* data, size_t size);

		bool Read(Buffer& mbuff, size_t readcap);
		bool Read(String& strbuff);
		bool ReadLine(Buffer& mbuff, size_t readcap);
		bool ReadLine(String& strbuff);
	private: //pImpl
		struct Impl;
		UniquePtr<Impl> pImpl;
	};
}