#include "MIBuffer.h"
#include "MIAssert.h"
#include "MIStringUtils.h"
#include "MIUtilities.h"

#include <iostream>

using namespace MI;
Buffer::Buffer() : pBuff{ nullptr }, size{ 0 } {}

	Buffer::Buffer(const int& cap) : pBuff{ new char[cap] {} } {
	}
	Buffer::Buffer(const int& cap, const char* str, size_t len) : pBuff{ new char[cap] {} } {
		strcpy(pBuff, len, str);
		size = len;
	}
	Buffer::Buffer(Buffer&& nbuff) : pBuff{ nbuff.pBuff }, size{ nbuff.size } {
		nbuff.pBuff = nullptr;
		nbuff.size = 0;
	}
	Buffer::~Buffer() {
		if (pBuff) { delete[] pBuff; }
	}
	void Buffer::SetSize(const int len) { size = len; *(pBuff + size) = '\0'; }

	int Buffer::Size() const noexcept { return size; }

	void Buffer::Clear() {
		size = 0;
		*pBuff = '\0';
	}

	void Buffer::Resize(const int& newCap) {
		char* newbuff = new char[newCap] {};
		if (!newbuff) {
			AssertMsg(0, "bad allocation");
			return; //error
		}
		strcpy(newbuff, size, pBuff);
		delete[] pBuff;
		pBuff = newbuff;
	}

	const char* Buffer::c_str() const { return pBuff; }
	char* Buffer::RawMemory() { return pBuff; }

	const void Buffer::AddEnd(const char* data, int writelen) {
		char* endBuff = pBuff + size;
		size += writelen;
		for (; writelen > 0; writelen--, endBuff++, data++) {
			*endBuff = *data;
		} 
		*endBuff = '\0';
	}

	void Buffer::operator=(const char* str) {
		Clear();
		AddEnd(str, strlen(str));
	}

	void Buffer::operator=(const Buffer& buff) {
		Clear();
		AddEnd(buff.pBuff, buff.size);
	}

	void Buffer::operator=(Buffer&& buff) {
		Swap(pBuff, buff.pBuff);
		Swap(size, buff.size);
	}

	bool Buffer::operator==(const Buffer& other) const{
		if (this->size != other.size) {
			return false;
		}
		else {
			for (int i = 0; i < size; i++) {
				if (*(pBuff + i) != *(other.pBuff + i)) {
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
			if (*(pBuff + i) < *(other.pBuff + i)) {
				return true;
			}
			else if (*(pBuff + i) > *(other.pBuff + i)) {
				return false;
			}
		}
		return (i != other.size);
	}

	bool Buffer::operator>(const Buffer& other) const {
		int i = 0;
		for (; i < Minimum(size, other.size); i++) {
			if (*(pBuff + i) > *(other.pBuff + i)) {
				return true;
			}
			else if (*(pBuff + i) < *(other.pBuff + i)) {
				return false;
			}
		}
		return (i != size);//indicates size was larger than other
	}

	bool Buffer::operator<=(const Buffer& other) const {
		int i = 0;
		for (; i < Minimum(size, other.size); i++) {
			if (*(pBuff + i) < *(other.pBuff + i)) {
				return true;
			}
			else if (*(pBuff + i) > *(other.pBuff + i)) {
				return false;
			}
		}
		return (size <= other.size);
	}

	bool Buffer::operator>=(const Buffer& other) const {
		int i = 0;
		for (; i < Minimum(size, other.size); i++) {
			if (*(pBuff + i) < *(other.pBuff + i)) {
				return true;
			}
			else if (*(pBuff + i) > *(other.pBuff + i)) {
				return false;
			}
		}
		return (size >= other.size);
	}

	void Buffer::WriteTo(FILE* file) {
		Assert(size == fwrite(pBuff, 1, size, file));
		Clear();
	}

	size_t Buffer::ReadFrom(FILE* file, size_t maxtoread) {
		return fread(pBuff, 1, maxtoread, file);
	}