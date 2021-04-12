#pragma once
namespace MI {
	//get the length of a cstring
	int strlen(const char* str);
	//unchecked copy of src string to dest string
	void strcpy(char* dest, int len, const char* src);
}