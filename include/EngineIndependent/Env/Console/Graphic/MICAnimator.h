#pragma once
//#include "MICImage.h"
#include "MIVector.h"
namespace MI {
	//Arbitrary rules for simplicity:
	// 1. Windows shall not have animators relative to other windows, only simple functionality for changing stack order/hiding, etc.
	// 2. Animations are limited to within a distinct window.
	// 3.	-Windows shall be notified that they are hidden (good animators will avoid
	//		updating the image until necessary, only updating their (probably) optimal intrinsic structure)
	// 4.	-Windows shall be notified of any regions that do not need display. (good animators will not include this in the Image).

	//CAnimator: One of the few classes I really do plan on making virtual
	//	Animations can span a fairly wide range of complexity. any solution I can think of without virtualization
	//	seems either like not enough for some cases (that I want to work), or seems well too much for other cases
	
	class CAnimator {
	protected:
		Vector<Image*> components;
	public:
		void Update(bool ishidden);
	};
	
	//CVisual: A type handling both animated and unanimated Images.
	//	Ideal usage: A CAnimator spawns CVisual objects under its control and lifetime. 
	//		Something (at least) with connection to a derived CAnimator implements physics, this class
	struct CVisual {
		CAnimator* animator = nullptr; 
		Image img;

		//Set an animator for this image
		void SetAnimator(CAnimator* animator);
		void SetImage(Image other);
		Image& GetImage();
		CAnimator* GetAnimatorPointer();
	};
}