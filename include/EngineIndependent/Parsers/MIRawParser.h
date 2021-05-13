#pragma once
#include <type_traits>
//variadic helper for passing both single values and ranges in a sequence for SkipSeq
namespace MI {

/*RawParser:
	A Fast, flexible parser. Good to use when you want a basic interface to use in a more detailed parser
	 instead of writing a giant procedural birds nest of pointer arithmetic and comparisons.
	-When using Isolation, you should use it by calling MarkIsolationPoint, parsing pstr, 
		then calling IsolateHere, using it's return string, then calling UnIsolate BEFORE you continue parsing.
		-If you do not call UnIsolate after calling IsolateHere, you will be left with a modified string (& parsing may not work as you hope)
*/
struct RawParser {
	char * pstr;
	
	void BeginParsing(char * str);
	
	char operator*();
	
	//Set the start of the string returned upon the next call to IsolateHere()
	void MarkIsolationPoint();
	
	/*set char at current position in the parser to '\0', returns the string starting at
	position of parser during last call to MarkIsolationPoint. */
	char* IsolateHere();
	
	//Replaces the character that was swapped with '\0' in call to IsolateHere
	void UnIsolate();
	
	//Advances the parses the by the given number of characters (bytes).
	void Advance(int totalbytes);
	//moves the parser up 1 character.
	void operator++(int);
	//True when the parsers current character is in the character range between the given low and high characters
	bool IsInRange(const char low, const char high);
	//keep parsing (skip) while current character is in the given range.
	void SkipRange(const char low, const char high);
	
	//True if the given character value is the same as the parsers current character's value.
	bool IsAtValue(const char v);
	//keep parsing (skip) while the current character value is the same as the one given.
	void SkipValue(const char v);
	
	/*if the initial substring (from current position of parser) compares equal to the given string
	 then returns the size, else returns 0. */
	int inline IsAtString(const char* v);
	//if the initial substring is the given string, skips it. (just once, unlike other "Skip" methods)
	bool SkipString(const char* v);
	
	
	/*Provided either a character, or a structure of two characters:
		1. If it's a character it skips any sequential occurences of the value.
		2. If it's two characters, it treats it like a range and skips any values between them. Ex. {'a', 'z'} skips lowercase letters */
	template <typename T>
	void Skip (T obj);
	
	//skip a sequence of ranges and/or values (Ex. SkipSeq(' ', {'A', 'Z'}, {'a', 'z'});)
	template <typename T, typename ...Args>
	void SkipSeq(T arg, Args ... args);
	
	/*Implementation thing: checks that a single condition holds (format like Skip method, returns true if the parsing string
		 is the given character, or in the range of the given range in case of a two-element structure)*/
	template <typename T>
	bool CheckCondition(T arg);
	
	/*Implementation thing: True if CheckCondition is true for any argument*/
	template <typename T, typename ...Args>
	bool CheckAnyOfConditions(T arg, Args ... args);
	
	/*Skips to the next value that is not any of the given character values, and is not in any given range.*/
	template <typename T, typename ... Args>
	void SkipAny(T arg, Args ... args);
private:
	char __swap;
	char * swapspot;
	char * isorv;
};
};
//*********************** Implementation details ****************************

/*template <typename T>
concept TwoMember = requires(volatile T obj1) { const auto& [a, b] {obj1}; };*/


template <typename T>
void MI::RawParser::Skip(const T obj) { 
	if constexpr (std::is_same_v<char, T>) {
		return SkipValue(obj);
	} else if constexpr(std::is_same_v<char*,T>) {
		return SkipString(obj);
	} else {
		const auto& [f, l] = obj;
		SkipRange(f, l);
	}
}

//skip a sequence of ranges and/or values (Ex. SkipSeq(' ', {'A', 'Z'}, 
template <typename T, typename ...Args>
void MI::RawParser::SkipSeq(T arg, Args ... args) {
	Skip(arg);
	if constexpr (sizeof...(Args) > 0) {
		SkipSeq(args...);
	}
}

template <typename T>
bool MI::RawParser::CheckCondition(T arg) {
	if constexpr (std::is_same_v<T, char>) {
		return IsAtValue(arg);
	} else {
		const auto& [f, l] = arg;
		return IsInRange(f,l);
	}
}
	
template <typename T, typename ... Args>
bool MI::RawParser::CheckAnyOfConditions(T arg, Args ... args) {
	if (CheckCondition(arg)) {
		return true;
	} else {
		if constexpr (sizeof...(Args) > 0) {
			return CheckAnyOfConditions(args...);
		} else {
			return false;
		}
	}
}
	
template <typename T, typename ... Args>
void MI::RawParser::SkipAny(T arg, Args ... args) {
	for(; CheckAnyOfConditions(arg, args...) ; ) {
		SkipSeq(arg, args...);
	}
}

