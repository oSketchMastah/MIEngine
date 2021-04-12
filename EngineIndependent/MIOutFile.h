#pragma once
#include "UniquePtr.h"

class FILE;

namespace MI {
	class Buffer;
	class FMode;

	struct OutFile {
		//setup buffer, and attach it to a file stream (if one is given to constructor)
		OutFile(const int bufcap = 16384);
		//open the file new file, OutFile becomes the owner of 
		OutFile(const char* filename, const FMode& mode, const int bufcap = 16384);
		OutFile(FILE* cfile, const int bufcap = 16384);
		OutFile(OutFile&& other); //move the resource from other file to this one
		~OutFile();

		//opens a new file with the specified mode, releasing previous file if necessary
		void Open(const char* filename, const FMode& mode);
		//Grabs the new file, assumes that it's writable and uses it for future writing operations
		void Open(FILE* aggregateFile, const FMode& mode);
		//sets up file buffer
		void Close();

		void Write(const Buffer& data);
		void WriteLine(const Buffer& data, const char* lineEnd = "\n");
		void Write(const char* data, size_t size);
	private:
		struct Impl;
		UniquePtr<Impl> pImpl;
	};
}