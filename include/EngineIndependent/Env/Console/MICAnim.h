#pragma once
#include "MICImage.h"
namespace MI {

	struct CAnimation {
		void (*imgmod) (Image&) = nullptr;
		Image img;

		//Takes as argument a function that modifies an Image to 
		void SetAnimator(void (*) (Image&));
	};
}