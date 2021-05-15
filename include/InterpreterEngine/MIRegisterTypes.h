#pragma once
#include "MIBank.h"
#include "MISymbolBank.h"
#include "MITypeHashMap.h"
#include "MITypeData.h"

namespace MI {
	
	//The flavour of map I'm going with for the type registry.
	template <typename KEY, typename VALUE>
	using Map = HashMap<KEY, VALUE>;



	static constexpr int nodesize = (sizeof(BankString) + sizeof(TypeData*)) + BYTEALIGN - (sizeof(BankString) + sizeof(TypeData)) % BYTEALIGN
	int Hash(BankString symbol); {
		int rv = 0;
		const char* temp = symbol.str;
		for (; *temp != '\0' ; temp++) {
			rv = 
		}
	};
	
	struct TypeRegistry {
		static SymbolBank names;
		static Bank data;
		static Initialize(int symbytes, int databytes) { names.Initialize(symbytes); data.Initialize(symbytes); }
		static Map<BankString, TypeData*> map;
		
		template<typename T>
		static void Register(const char* type) {
			BankString bs = names.Allocate(type);
			TypeData* td = data.Allocate<TypeData>();
			
			map.Add(td
		}
	};
	
};
