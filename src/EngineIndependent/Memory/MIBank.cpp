#include "MIBank.h"
#include <stdlib.h>
using namespace MI;
	//allocates an aligned bank of memory
void Bank::Initialize(int bytesize, int bytealign) {
#if defined(WIN32) || defined(WIN64)
	allocbank = calloc(1, bytesize);
	bank = allocbank;
	unsigned int curralign = (unsigned int)(reinterpret_cast<char*>(allocbank)) % bytealign;
	bank = reinterpret_cast<char*>(allocbank) + bytealign - curralign;
#else
	bank = aligned_alloc(bytealign, bytesize);
#endif
	end = bank;
}

Bank::~Bank() {
#if defined(WIN32) || defined(WIN64)
	free(allocbank);
#else
	free(bank);
#endif
}
