#pragma once

#include "MIStringUtils.h"
#include "MIBank.h"
namespace MI {
	//struct Bank;
	struct BankString {
		char* str;
			
		void Initialize(Bank& bank, const char* setv) {
			unsigned int rawlen = strlen(setv) + 1;
			str = bank.Allocate<char>(rawlen);
			strcpy(str, rawlen, setv);
		};
		
		bool operator==(const char* other) const;
		bool operator!=(const char* other) const;
		bool operator<(const char* other) const;
		bool operator>(const char* other) const;
		bool operator<=(const char* other) const;
		bool operator>=(const char* other) const;
	};
};
