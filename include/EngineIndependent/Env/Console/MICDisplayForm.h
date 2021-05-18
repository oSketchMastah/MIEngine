#pragma once
#include "MIUniquePtr.h"
#include "MIBank.h"//probably will need something more specific to a single terminal
struct Bank;
struct DisplayForm {
	Bank& displaybank;
};