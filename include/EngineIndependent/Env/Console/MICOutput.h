#pragma once
#include "MICBasicAnim.h"
#include "MILinkedList.h"
struct ConsoleOutput {
	BasicAnimation* bganim;
	DLinkList<DLinkList<BasicAnimation*>> 
};