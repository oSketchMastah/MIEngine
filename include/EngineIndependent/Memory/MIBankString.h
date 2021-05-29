#pragma once
namespace MI {
	//struct Bank;
	struct BankString {
		const char* str;
		operator const char* () const;
		bool operator==(const char* other) const;
		bool operator!=(const char* other) const;
		bool operator<(const char* other) const;
		bool operator>(const char* other) const;
		bool operator<=(const char* other) const;
		bool operator>=(const char* other) const;
	};
};
