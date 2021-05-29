#include "MISymbolBank.h"
#include "MIStringUtils.h"
#include <cstdlib>
using namespace MI;

void SymbolBank::Initialize(int bytesize, int bytealign) {
#if defined(WIN32) || defined(WIN64)
	allocbank = malloc(bytesize);
	bank = allocbank;
	unsigned int curralign = (unsigned int)(reinterpret_cast<char*>(allocbank)) % bytealign;
	bank = reinterpret_cast<char*>(allocbank) + bytealign - curralign;
#else
	bank = aligned_alloc(bytealign, bytesize);
#endif
	end = bank;
}

SymbolBank::~SymbolBank() {
#if defined(WIN32) || defined(WIN64)
	free(allocbank);
#else
	free(bank);
#endif
}

BankString SymbolBank::Allocate(const char* str) {
	int neededsize = strlen(str) + 1;
	char* m_alloc = reinterpret_cast<char*>(end);
	strcpy(m_alloc, neededsize , str);
	end = reinterpret_cast<char*>(end) + neededsize + (BYTEALIGN - neededsize % BYTEALIGN);
	return BankString{m_alloc};
}
