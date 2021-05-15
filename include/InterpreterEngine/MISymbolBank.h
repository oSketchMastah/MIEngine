#include "MIBankString.h"
#define BYTEALIGN 8
namespace MI {
	class SymbolBank {
	protected:
		void * bank;
		void * end;
	public:
		//allocates an aligned bank of memory
		void Initialize(int bytesize, int bytealign = BYTEALIGN);
		//frees the memory allocated for the bank
		~SymbolBank();
		//Allocate a string onto the bank, respecting alignment requirements.
		BankString Allocate(const char* str);
	};
}
