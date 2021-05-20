#pragma once
#include "MICVisual.h"
#include "MIBounds.h"
namespace MI {
	//By assigning layers and functionality here, we can 
	struct CWindow {
		Map<int, CVisual> inner;
		Bounds<2, int> winbounds;
		char border = 0;
	};
}