#pragma once
namespace MI {
	//struct Bank;
	struct BankString {
		const char* str;
		
		bool operator==(const char* other) const;
		bool operator!=(const char* other) const;
		bool operator<(const char* other) const;
		bool operator>(const char* other) const;
		bool operator<=(const char* other) const;
		bool operator>=(const char* other) const;
	};
};
