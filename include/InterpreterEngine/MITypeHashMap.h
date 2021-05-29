#pragma once
#define BYTEALIGN 8
#include "MIBankString.h"
namespace MI {

struct Node {
	
}
struct TypeHashMap {
	static constexpr NodeTotal = 100;
	TypeHashMap(int nodetotal, int bytealign = BYTEALIGN);
};

}
