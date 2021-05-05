#include "MIBuffer.h"
#include "MIAssert.h"
#include "MIStringUtils.h"
#include "MIUtilities.h"

#include <iostream>

using namespace MI;
Buffer::Buffer() : buff{ nullptr }, size{ 0 } {}

	Buffer::Buffer(const int& cap) : buff{ new char[cap] {} } {
	}
	Buffer::Buffer(const int& cap, const char* str, size_t len) : buff{ new char[cap] {} } {
		strcpy(buff, len, str);
		size = len;
	}
	Buffer::Buffer(Buffer&& nbuff) : buff{ nbuff.buff }, size{ nbuff.size } {
		nbuff.buff = nullptr;
		nbuff.size = 0;
	}
	Buffer::~Buffer() {
		if (buff) { delete[] buff; }
	}

	Buffer& Buffer::Get() {
		return *this;
	}
	void Buffer::SetSize(const int len) { size = len; *(buff + size) = '\0'; }

	int Buffer::Size() const noexcept { return size; }

	void Buffer::Clear() {
		if (buff) {
			size = 0;
			*buff = '\0';
		}
	}

	void Buffer::Resize(const int& newCap) {
		char* newbuff = new char[newCap] {};
		if (!newbuff) {
			AssertMsg(0, "bad allocation");
			return; //error
		}
		strcpy(newbuff, size, buff);
		delete[] buff;
		buff = newbuff;
	}
	Buffer::operator const char* () const {
		return buff;
	}
	const char* Buffer::c_str() const { return buff; }
	const char* Buffer::end() const { return buff + size; }
	char* Buffer::RawMemory() { return buff; }

	const void Buffer::AddEnd(const char* data, int writelen) {
		char* endBuff = buff + size;
		size += writelen;
		for (; writelen > 0; writelen--, endBuff++, data++) {
			*endBuff = *data;
		} 
		*endBuff = '\0';
	}

	Buffer& Buffer::operator=(const Buffer& other) {
		Clear();
		AddEnd(other.buff, other.size);
		return *this;
	}

	Buffer& Buffer::operator=(Buffer&& other) {
		Swap(buff, other.buff);
		Swap(size, other.size);
		return *this;
	}

	bool Buffer::operator==(const Buffer& other) const {
		if (this->size != other.size) {
			return false;
		}
		else {
			for (int i = 0; i < size; i++) {
				if (*(buff + i) != *(other.buff + i)) {
					return false;
				}
			}
		}
		return true;
	}

	bool Buffer::operator!=(const Buffer& other) const {
		return !(*this == other);
	}

	const bool Minimum(size_t x, size_t y) {
		return (x < y) ? x : y;
	}

	bool Buffer::operator<(const Buffer& other) const {
		int i = 0;
		for (; i < Minimum( size, other.size ); i++) {
			if (*(buff + i) < *(other.buff + i)) {
				return true;
			}
			else if (*(buff + i) > *(other.buff + i)) {
				return false;
			}
		}
		return (i != other.size);
	}

	bool Buffer::operator>(const Buffer& other) const {
		int i = 0;
		for (; i < Minimum(size, other.size); i++) {
			if (*(buff + i) > *(other.buff + i)) {
				return true;
			}
			else if (*(buff + i) < *(other.buff + i)) {
				return false;
			}
		}
		return (i != size);//indicates size was larger than other
	}

	bool Buffer::operator<=(const Buffer& other) const {
		int i = 0;
		for (; i < Minimum(size, other.size); i++) {
			if (*(buff + i) < *(other.buff + i)) {
				return true;
			}
			else if (*(buff + i) > *(other.buff + i)) {
				return false;
			}
		}
		return (size <= other.size);
	}

	bool Buffer::operator>=(const Buffer& other) const {
		int i = 0;
		for (; i < Minimum(size, other.size); i++) {
			if (*(buff + i) > *(other.buff + i)) {
				return true;
			}
			else if (*(buff + i) < *(other.buff + i)) {
				return false;
			}
		}
		return (size >= other.size);
	}

	void Buffer::WriteTo(FILE* file) {
		Assert(size == fwrite(buff, 1, size, file));
		Clear();
	}

	size_t Buffer::ReadFrom(FILE* file, size_t maxtoread) {
		return fread(buff, 1, maxtoread, file);
	}