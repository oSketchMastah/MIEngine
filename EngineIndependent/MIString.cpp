#include "MIString.h"
#include "MIStringUtils.h"
#include "MIUtilities.h"
#include "MIAssert.h"

#include <memory>

using namespace MI;

size_t String::Size() const {
	return buff.Size();
}
size_t String::Capacity() const {
	return capacity;
}

void String::ResizeToFit(const size_t total) {
	size_t newsize = (capacity != 0) ? 2 * capacity : 1;
	for (; newsize < total; newsize *= 2) {
		continue;//needs resize
	}

	buff.Resize(newsize);
	this->capacity = newsize;
}

void inline String::Shift(const size_t offset) {
	char* pc = buff.RawMemory() + buff.Size() + offset;
	for (; pc != buff.RawMemory() + offset ; pc--) {
		*pc = *(pc - offset);
	}
	buff.SetSize(buff.Size() + offset);
}

bool String::NeedsResizeToAdd(size_t charsToAdd) {
	return capacity <= buff.Size() + charsToAdd;
}

/*implicit treatment of a string as it's underlying buffer,
	nothing external to a string object needs capacity data*/
String::operator const Buffer& () const {
	return buff;
}
String::operator const char* () const {
	return buff;
}

Buffer& String::GetBuffer() {
	return buff;
}
template <typename T>
T exp2Ceil(T num) {
	int i = 0;
	for (; num != 0; i++) {
		num = num / 2;
	} //i gives the number of shifts to zero out num (index of the highest 1 bit + 1)
	return (i != 0) ? (1 << i) : 0;
}

String::String() : capacity(0), buff() {

}
String::String(String&& str) : capacity(str.capacity), buff(std::move(str.buff)) { //copy constructors

}
String::String(const String& str) { //copy constructors
	capacity = exp2Ceil(str.Size());
	buff = std::move(Buffer(capacity, str.c_str(), str.Size()));
}

String::String(const Buffer& nbuff) {
	capacity = exp2Ceil(nbuff.Size());
	buff = std::move(Buffer(capacity, nbuff.c_str(), nbuff.Size()));
}

String::String(const char* str) : capacity(exp2Ceil(strlen(str))) {
	buff = std::move(Buffer(capacity, str, strlen(str)));
}


String::~String() {
}

const char* String::c_str() const {
	return buff.c_str();
}
#include <iostream>

//Append a single character to the end, pls try and add maximal amounts.
void String::operator=(char c) {
	if (capacity < 1) {
		ResizeToFit(1);
	}
	buff.Clear();
	buff.AddEnd(&c, 1);
}
//Append the cstring to the end of this string
void String::operator=(const char* str) {
	const int len = strlen(str);
	if (capacity < len) {
		ResizeToFit(len);
	}
	buff.Clear();
	buff.AddEnd(str, len);
}
//append the buffer's data to the end of this string
void String::operator=(const Buffer& nbuff) {
	if (capacity < nbuff.Size()) {
		ResizeToFit(nbuff.Size());
	}
	buff.Clear();
	buff.AddEnd(nbuff.c_str(), nbuff.size);
}
//
void String::operator=(String&& nstr) {
	Swap(buff, nstr.buff);
	Swap(capacity, nstr.capacity);
}

//Append a single character to the end, pls try and add maximal amounts.
void String::operator+=(const char c) {
	if (this->capacity < this->Size() + 1) {//needs resize
		ResizeToFit(this->Size() + 1);
	}
	buff.AddEnd(&c, 1);
}
//Append the cstring to the end of this string
void String::operator+=(const char* str) {
	const int len = strlen(str);
	if (this->capacity < this->Size() + len) {//needs resize
		ResizeToFit(this->Size() + len);
	}
	buff.AddEnd(str, len);
}
//append the buffer's data to the end of this string
void String::operator+=(const Buffer& nbuff) {
	if (this->capacity < this->Size() + nbuff.Size()) {//needs resize
		ResizeToFit(this->Size() + nbuff.Size());
	}
	buff.AddEnd(nbuff.c_str(), nbuff.Size());
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

String MI::operator+(String& left, String const& right) {
	String newstr(left);
	if (newstr.Capacity() < left.Size() + right.Size()) { //use other buffer if it's bigger
		newstr.ResizeToFit(left.buff.size + right.Size());
	}
	newstr.buff.AddEnd(right.c_str(), right.Size());
	return std::move(newstr);
}

String MI::operator+(String&& left, String const& right) {
	if (left.Capacity() < left.Size() + right.Size()) { //use other buffer if it's bigger
		left.ResizeToFit(left.buff.size + right.Size());
	}
	left.buff.AddEnd(right.c_str(), right.Size());
	return std::move(left);
}

String MI::operator+(String& left, const char* right) {
	String newstr(left);
	int rlen = strlen(right);
	if (newstr.Capacity() < left.Size() + rlen) { //use other buffer if it's bigger
		newstr.ResizeToFit(left.buff.size + rlen);
	}
	newstr.buff.AddEnd(right, rlen);
	return std::move(newstr);
}

String MI::operator+(String&& left, const char* right) {
	int rlen = strlen(right);
	if (left.Capacity() < left.Size() + rlen) { //use other buffer if it's bigger
		left.ResizeToFit(left.buff.size + rlen);
	}
	left.buff.AddEnd(right, rlen);
	return std::move(left);
}


