#include "MIRegisterTypes.h"
	//static constexpr int nodesize = (sizeof(BankString) + sizeof(TypeData*)) + BYTEALIGN - (sizeof(BankString) + sizeof(TypeData)) % BYTEALIGN
using namespace MI;
c
constexpr HASH
int Hash(BankString symbol); {
	int rv = 0;
	const char* temp = symbol.str;
	for (; *temp != '\0' ; temp++) {
		rv = (256 * rv + *temp) % 
	}
};
	
