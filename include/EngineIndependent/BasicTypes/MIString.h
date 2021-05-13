#pragma once
#include "MIBuffer.h"
namespace MI {
	class WideString; //make use of unicode
	class FinalString; //Save memory with strings allocated in a special finalization stack

	/* String: is a buffer with capacity data 
		for added safety and dynamic resizing
	*/
	class String : public Buffer { //Buffer holds size and data
		size_t capacity;
		/*Resizes to fit at least numC characters + 1 null terminator. The edge case
		*	here being when you give 2^n for some n, and it resizes to 2^(n+1) */
		void ResizeToFit(size_t numC);
		//Shifts
		void Shift(size_t numC);
	public:
		bool NeedsResizeToAdd(size_t charsToAdd);
		size_t Capacity() const noexcept; 
		
		/*implicit treatment of a string as it's underlying buffer,
			nothing external to a string object needs capacity data*/
		Buffer& GetBuffer();

		String();
		String(String&&);
		String(const String&);//copy constructors
		String(const Buffer&);//copy constructors
		String(const char*);

		~String();

		//the operators should NOT need to take strings, the required data comes from data and size
		//Append a single character to the end, pls try and add maximal amounts.
		String& operator=(char);
		//Append the cstring to the end of this string
		String& operator=(const char*);
		//append the buffer's data to the end of this string
		String& operator=(const Buffer &);
		String& operator=(const String& str); //

		/*swap/move the buffer from other to this, note that this should not be valid for Buffer&&, 
			since accurate capacity data is needed*/
		String& operator=(String&&) noexcept;

		//Append a single character to the end, pls try and add maximal amounts.
		void operator+=(const char);
		//Append the cstring to the end of this string
		void operator+=(const char*);
		//append the buffer's data to the end of this string
		void operator+=(const Buffer &);
		//void operator+=(const String&);
		friend String operator+(const String& left, String const& right);
		friend String operator+(String&& left, String const& right);
		friend String operator+(const String& left, const char*);
		friend String operator+(String&& left, const char*);

		/**Finalizers - reduce memory usage by allocating string in special buffer, we then only need to
			  reference the string at that location + get safe noexcept and const policies*/

		//Copies all the data over to a buffer with just enough space for the data + null terminator
		//const FinalString& Finalize();
		//Copies all the data over to a buffer with enough space to hold the given capacity (cap)
		//const FinalString& Finalize(size_t cap);


		//converts this ascci string into a wide character string
		//const WideString& ToWide() const;

	};
	//String& operator+(String& left, String const& right);
}