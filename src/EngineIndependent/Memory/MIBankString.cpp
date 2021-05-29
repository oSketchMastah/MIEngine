#include "MIBankString.h"
using namespace MI;

BankString::operator const char* () const {
	return str;
}

bool BankString::operator==(const char* other) const {
	const char* pc = str;
	for (; *pc == *other && *other != '\0' ; pc++, other++) { continue; }
	return *pc == *other;
}

bool BankString::operator!=(const char* other) const {
	const char* pc = str;
	for (; *pc == *other && *other != '\0' ; pc++, other++) { continue; }
	return *pc != *other;
}

bool BankString::operator<(const char* other) const {
	const char* pc = str;
	for (; *pc == *other && *other != '\0' ; pc++, other++) { continue; }
	return *pc < *other;
}

bool BankString::operator>(const char* other) const {
	const char* pc = str;
	for (; *pc == *other && *other != '\0' ; pc++, other++) { continue; }
	return *pc > *other;
}

bool BankString::operator<=(const char* other) const {
	const char* pc = str;
	for (; *pc == *other && *other != '\0' ; pc++, other++) { continue; }
	return *pc <= *other;
}

bool BankString::operator>=(const char* other) const {
	const char* pc = str;
	for (; *pc == *other && *other != '\0' ; pc++, other++) { continue; }
	return *pc >= *other;
}

