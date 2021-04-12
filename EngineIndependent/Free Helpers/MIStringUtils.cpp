#include "MIStringUtils.h"
namespace MI {
	int strlen(const char* str) {
		int len = 0;
		for (; *str != '\0'; str++) { len++; }
		return len;
	}
	//not checked, make sure dest can fit len from src (and that src has this length)
	void strcpy(char* dest, const int len, const char* src) {
		for (int i = 0; i < len; i++, src++, dest++) { *dest = *src; }
	}
}