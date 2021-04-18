#include "MIString.h"
#include "MIStringUtils.h"
#include "MIUtilities.h"
#include "MIAssert.h"

#include <memory>

using namespace MI;

size_t String::Capacity() const noexcept {
	return capacity;
}


void String::ResizeToFit(const size_t total) {
	size_t newsize = (capacity != 0) ? 2 * capacity : 1;
	for (; newsize <= total; newsize *= 2) {
		continue;//needs resize
	}
	Buffer::Resize(newsize);
	//buff.Resize(newsize);
	this->capacity = newsize;
}

void inline String::Shift(const size_t offset) {
	char* pc = Buffer::RawMemory() + Buffer::Size() + offset;
	for (; pc != Buffer::RawMemory() + offset ; pc--) {
		*pc = *(pc - offset);
	}
	Buffer::SetSize(Buffer::Size() + offset);
}

bool String::NeedsResizeToAdd(size_t charsToAdd) {
	return capacity <= Buffer::Size() + charsToAdd;
}

/*implicit treatment of a string as it's underlying buffer,
	nothing external to a string object needs capacity data*/

Buffer& String::GetBuffer() {
	return Buffer::Get();
}
template <typename T>
T exp2Ceil(T num) {
	int i = 0;
	for (; num != 0; i++) {
		num = num / 2;
	} //i gives the number of shifts to zero out num (index of the highest 1 bit + 1)
	return (i != 0) ? (1 << i) : 0;
}

String::String() : capacity(0), Buffer() {

}

String::String(String&& str) : capacity(str.capacity), Buffer(std::move(str.GetBuffer())) { //copy constructors

}
String::String(const String& str) { //copy constructors
	capacity = exp2Ceil(str.Size());
	Buffer::Get() = std::move(Buffer(capacity, str, str.Size()));
}

String::String(const Buffer& nbuff) {
	capacity = exp2Ceil(nbuff.Size());
	Buffer::Get() = std::move(Buffer(capacity, nbuff, nbuff.Size()));
}

String::String(const char* str) : capacity(exp2Ceil(strlen(str))) {
	Buffer::Get() = std::move(Buffer(capacity, str, strlen(str)));
}


String::~String() {
}

#include <iostream>

//Append a single character to the end, pls try and add maximal amounts.
String& String::operator=(char c) {
	if (capacity <= 1) {
		ResizeToFit(1);
	}
	Buffer::Clear();
	Buffer::AddEnd(&c, 1);
	return *this;
}
//Append the cstring to the end of this string
String& String::operator=(const char* str) {
	const int len = strlen(str);
	if (capacity <= len) {
		ResizeToFit(len);
	}
	Buffer::Clear();
	Buffer::AddEnd(str, len);
	return *this;
}
//append the buffer's data to the end of this string
String& String::operator=(const Buffer& nbuff) {
	if (capacity <= nbuff.Size()) {
		ResizeToFit(nbuff.Size());
	}
	Buffer::Clear();
	Buffer::AddEnd(nbuff, nbuff.Size());
	return *this;
}

//append the buffer's data to the end of this string
String& String::operator=(const String& other) {
	if (capacity <= other.Size()) {
		ResizeToFit(other.Size());
	}
	Buffer::Clear();
	Buffer::AddEnd(other, other.Size());
	return *this;
}
//
String& String::operator=(String&& nstr) noexcept {
	Swap(GetBuffer(), nstr.GetBuffer());
	Swap(capacity, nstr.capacity);
	return *this;
}


//Append a single character to the end, pls try and add maximal amounts.
void String::operator+=(const char c) {
	if (this->capacity <= this->Size()) {//needs resize
		ResizeToFit(this->Size() + 1);
	}
	Buffer::AddEnd(&c, 1);
}
//Append the cstring to the end of this string
void String::operator+=(const char* str) {
	const int len = strlen(str);
	if (this->capacity <= this->Size() + len) {//needs resize
		ResizeToFit(this->Size() + len);
	}
	Buffer::AddEnd(str, len);
}
//append the buffer's data to the end of this string
void String::operator+=(const Buffer& nbuff) {
	if (this->capacity <= this->Size() + nbuff.Size()) {//needs resize
		ResizeToFit(this->Size() + nbuff.Size());
	}
	Buffer::AddEnd(nbuff, nbuff.Size());
}
/*
void String::operator+=(const String& str) {
	if (this->capacity < this->Size() + str.Size()) {//needs resize
		buff.Resize(2 * this->capacity);
	}
	buff.AddEnd(str.c_str(), str.Size());
}*/


/*String& String::operator+(const char* str) {
	const int len = strlen(str);
	if (capacity < this->Size() + len) {
		ResizeToFit(this->Size() + len);
	}
	buff.AddEnd(str, len);
	return std::move(*this);
}*/

/*String&& String::operator+(const String& str) {//string + copy lvalue string -> rvalue string
	if (this->capacity < this->buff.size + str.Size()) { //use other buffer if it's bigger
		ResizeToFit(this->buff.size + str.Size());
	}
	buff.AddEnd(str.c_str(), str.Size());
	return std::move(*this);
}*/

/*String&& String::operator+(String&& temp) {//string + moved rvalue string -> rvalue string
	if (Maximum(capacity, temp.capacity) < buff.size + temp.buff.size) {
		ResizeToFit(buff.size + temp.buff.size);
		buff.AddEnd(temp.buff.c_str(), temp.buff.size);
	}//After this, we know largest capacity string can fit the data
	else if (temp.capacity > this->capacity) { //use other buffer if it's bigger
		Swap(buff.pBuff, temp.buff.pBuff);
		Swap(buff.size, temp.buff.size);
		Swap(capacity, temp.capacity);
		Shift(temp.buff.size);
		strcpy(buff.pBuff, temp.buff.size, temp.buff.pBuff);
	}
	else {
		buff.AddEnd(temp.buff.c_str(), temp.buff.size);
	}

	return std::move(*this);
}*/

String MI::operator+(const String& left, String const& right) {
	String newstr(left);
	if (newstr.Capacity() <= left.Size() + right.Size()) { //use other buffer if it's bigger
		newstr.ResizeToFit(left.Size() + right.Size());
	}
	newstr.AddEnd(right, right.Size());
	return std::move(newstr);
}

String MI::operator+(String&& left, String const& right) {
	if (left.Capacity() <= left.Size() + right.Size()) { //use other buffer if it's bigger
		left.ResizeToFit(left.Size() + right.Size());
	}
	left.AddEnd(right, right.Size());
	return std::move(left);
}

String MI::operator+(const String& left, const char* right) {
	String newstr(left);
	int rlen = strlen(right);
	if (newstr.Capacity() <= left.Size() + rlen) { //use other buffer if it's bigger
		newstr.ResizeToFit(left.Size() + rlen);
	}
	newstr.AddEnd(right, rlen);
	return std::move(newstr);
}

String MI::operator+(String&& left, const char* right) {
	int rlen = strlen(right);
	if (left.Capacity() <= left.Size() + rlen) { //use other buffer if it's bigger
		left.ResizeToFit(left.Size() + rlen);
	}
	left.AddEnd(right, rlen);
	return std::move(left);
}


