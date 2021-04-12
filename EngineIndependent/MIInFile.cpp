#include "InFile.h"

#include "FileMode.h"
#include <cstdio>
using namespace MI;


struct InFile::Impl {

};

//setup buffer, and attach it to a file stream (if one is given to constructor)
InFile::InFile(const int bufcap) {

}
InFile::InFile(const char* filename, const FMode& mode, const int bufcap) {

}

InFile::InFile(const FILE* cfile, const FMode& mode, const int bufcap) {

};

InFile::InFile(InFile&& other) { //move the resource from other file to this one

}
InFile::~InFile(){}

//opens a new file with the specified mode, releasing previous file if necessary
void InFile::Open(const char* filename, const FMode& mode) {

}
//sets up file buffer
void InFile::Close() {

}

bool InFile::Read(Buffer& mbuff, size_t readcap) {

}
bool InFile::Read(String& strbuff) {

}
bool InFile::ReadLine(Buffer& mbuff, size_t readcap) {

}
bool InFile::ReadLine(String& strbuff) {

}
