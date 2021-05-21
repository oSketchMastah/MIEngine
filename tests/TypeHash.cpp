#include "MIStringHashMap.h"
#include "MIDetectionTests.h"
#include "MIConsole.h"
#include "MISymbolBank.h"

using namespace MI;

namespace MI {
#define TYPE_HASH_SIZE 100
	struct TypeInfo {
		inline static SymbolBank symbank;
		inline static Bank allocator;
		inline static StringHashMap<TypeInfo*, TYPE_HASH_SIZE> infomap;

		static void Initialize() {
			symbank.Initialize(8192);
			/*This Bank (allocator) will have enough capacity to handle storing the HashMap with TYPE_HASH_SIZE pointers, as well as any and all possible TypeInfo
			*	Note: you should adjust the value to be somewhat significantly larger than how many TypeInfo objects you plan on storing (reduces collisions).
			*/
			allocator.Initialize( (TYPE_HASH_SIZE) * ( sizeof(TypeInfo) + sizeof(TypeInfo*)  ) ); //this could be a bigger allocator used to allocate the hashmap
			infomap.Initialize(allocator);
		}
		static void RegisterClass(const char* name) {
			TypeInfo* pinfo = allocator.Allocate<TypeInfo>();
			pinfo->name = symbank.Allocate(name);
			infomap.Register(name, pinfo);
		}

		BankString name;

		TypeInfo(BankString banked) : name{ banked } {}

		bool operator==(const char* strname) {
			return name == strname;
		}
		bool operator!=(const char* strname) {
			return name != strname;
		}
	};
	static TypeInfo& GetTypeInfo(const char* str) {
		return *TypeInfo::infomap.HashStructure(str);
	}
}

struct A {
	static void Initialize() {
		TypeInfo::RegisterClass("A");
	}
};
bool Test() {
	TypeInfo::Initialize();
	A::Initialize();
	DISPROVE_CHECK( GetTypeInfo("A").name == "A" );
	return true;
}

int main(int argc, char* argv[]) {
	Console::Initialize();
	if (Test()) {
		return 0;
	}
	return 1;
}
