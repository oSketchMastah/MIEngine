#pragma once
#include <cstdio>

namespace MI {
	/*A minimal, unchecked buffer class, w/ char* data and size,  
		capacity is not tracked (you must do that yourself when using AddEnd) */
	struct Buffer {
	protected:
		size_t size = 0;
		char* buff = nullptr;
	public:
		Buffer(); //Uninitialized
		Buffer(const int& cap); //Initialized with specified cap
		Buffer(const int& cap, const char* str, size_t len);
		//copy
		//Buffer(const Buffer& other);
		Buffer(Buffer&& nbuff);
		~Buffer();

		Buffer& Get();
		//MUST CALL THIS WHEN A BUFFER MODIFYING PROCESS CHANGES THE LENGTH AND REPORTS (generally through return value)
		void SetSize(const int len);
		int Size() const noexcept;
		//Sets the size to 0 and null terminates the beginning of the buffer
		void Clear();
		//Deletes this buffer and replaces it with one of the specified capacity
		void Resize(const int& newCap);
		
		//The cstring buffer data, if the buffer is const, assume implicit
		//		cstring treatment is safe, otherwise should be explicit
		operator const char* () const;
		const char* c_str() const;

		//Get a reference to the memory location (char*) holding the buffered data
		char* RawMemory();
		//Add the specified byte length from the stringified data to the buffer (unchecked)
		const void AddEnd(const char* data, int len);

		//Assignment operators
		Buffer& operator=(const Buffer& buff);
		Buffer& operator=(Buffer&& buff);

		//Convenience/Mathematical operators
		bool operator==(const Buffer& other) const;
		bool operator!=(const Buffer& other) const;
		bool operator<(const Buffer& other) const;
		bool operator>(const Buffer& other) const;
		bool operator<=(const Buffer& other) const;
		bool operator>=(const Buffer& other) const;

		void WriteTo(FILE* file);
		size_t ReadFrom(FILE* file, size_t bytecap);
	};
}