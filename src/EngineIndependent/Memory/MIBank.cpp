#include "MIBank.h"
#include <stdlib.h>
using namespace MI;
	//allocates an aligned bank of memory
void Bank::Initialize(int bytesize, int bytealign) {
	bank = aligned_alloc(bytealign, bytesize);
	end = bank;
}

Bank::~Bank() {
	free(bank);
}
