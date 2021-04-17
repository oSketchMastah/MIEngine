#pragma once
#include "MIBuffer.h"
namespace MI {
	class WideString; //make use of unicode
	class FinalString; //Save memory with strings allocated in a special finalization stack

	class String {
		Buffer buff; //holds size and data
		size_t capacity; //tracks allocated size for dynamics
		void ResizeToFit(size_t numC);
		void Shift(size_t numC);
	public:
		bool NeedsResizeToAdd( size_t charsToAdd);
		size_t Size() const;
		size_t Capacity() const; 
		
		/*implicit treatment of a string as it's underlying buffer,
			nothing external to a string object needs capacity data*/
		operator const Buffer&() const;
		operator const char* () const;
		Buffer& GetBuffer();

		String();
		String(String&&);
		String(const String&);//copy constructors
		String(const Buffer&);//copy constructors
		String(const char*);
		//constexpr String(const char arr[]);

		~String();

		const char* c_str() const;


		//the operators should NOT need to take strings, the required data comes from data and size
		//Append a single character to the end, pls try and add maximal amounts.
		void operator=(char);
		//Append the cstring to the end of this string
		void operator=(const char*);
		//append the buffer's data to the end of this string
		void operator=(const Buffer &);
		/*swap/move the buffer from other to this, note that this should not be valid for Buffer&&, 
			since accurate capacity data is needed*/
		void operator=(String&&);

		//Append a single character to the end, pls try and add maximal amounts.
		void operator+=(const char);
		//Append the cstring to the end of this string
		void operator+=(const char*);
		//append the buffer's data to the end of this string
		void operator+=(const Buffer &);
		//void operator+=(const String&);
		friend String operator+(String& left, String const& right);
		friend String operator+(String&& left, String const& right);
		friend String operator+(String& left, const char*);
		friend String operator+(String&& left, const char*);

		/**Finalizers - reduce memory usage by allocating string in special buffer, we then only need to
			  reference the string at that location + get safe noexcept and const policies*/

		//Copies all the data over to a buffer with just enough space for the data + null terminator
		const FinalString& Finalize();
		//Copies all the data over to a buffer with enough space to hold the given capacity (cap)
		const FinalString& Finalize(size_t cap);


		//converts this ascci string into a wide character string
		const WideString& ToWide() const;

	};
	//String& operator+(String& left, String const& right);
}