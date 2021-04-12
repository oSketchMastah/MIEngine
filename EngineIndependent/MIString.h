#pragma once
#include "MIBuffer.h"
namespace MI {
	class String {
		Buffer buff; //holds size and data
		size_t capacity; //tracks allocated size for dynamics
		void ResizeToFit(size_t numC);
		void Shift(size_t numC);
	public:
		size_t Size() const;
		size_t Capacity() const; 
		
		/*implicit treatment of a string as it's underlying buffer,
			nothing external to a string object needs capacity data*/
		operator const Buffer&() const;
		Buffer& GetBuffer();

		String();
		String(String&&);
		String(const String&);//copy constructors
		String(const Buffer&);//copy constructors
		String(const char*);
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

		//inline String&& operator+( const char* cstr);
		//const String& operator+(const char*) const;
		//String operator+(const char*); //
		//String&& operator+(const String& temp); //string + copy lvalue string -> rvalue string
		//String&& operator+(String&& temp); //string + moved rvalue string -> rvalue string

		friend String operator+(String& left, String const& right);
		friend String operator+(String&& left, String const& right);

		friend String operator+(String& left, const char*);
		friend String operator+(String&& left, const char*);

	};
	//String& operator+(String& left, String const& right);
}