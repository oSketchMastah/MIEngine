#include "MISymbolBank.h"
#include "MIStringUtils.h"
#include <cstdlib>
using namespace MI;

void SymbolBank::Initialize(int bytesize, int bytealign) {
	bank = aligned_alloc(bytealign, bytesize);
	end = bank;
}

SymbolBank::~SymbolBank() {
	free(bank);
}

BankString SymbolBank::Allocate(const char* str) {
	int neededsize = strlen(str) + 1;
	char* m_alloc = reinterpret_cast<char*>(end);
	strcpy(m_alloc, neededsize , str);
	end = reinterpret_cast<char*>(end) + neededsize + (BYTEALIGN - neededsize % BYTEALIGN);
	return BankString{m_alloc};
}
