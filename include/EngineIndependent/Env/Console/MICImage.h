#pragma once
#include "MIRange.h"
#include "MILinkedList.h"
namespace MI {

	enum ColorEfx : int {
		Reset = -1,
		Black = 0,
		Red = 1,
		Green = 2,
		Yellow = 3,
		Blue = 4,
		Magenta = 5,
		Cyan = 6,
		White = 7,

		TXT = 30,
		BKG = 40
	};

	//an invalid range (where last is before first, will be disregarded, but ConsDiff may 
	// still have a ColorEfx specified that modifies color until a new one is specified.
	struct ConsDiff {
		ColorEfx effector{ 0 };
		Range<char> range;
		char c{ '\0' };
	};

	//Encode to a high-level diff encoding, for easy construction of a delayered composition
	struct Image {
		//const CWindow& win; //access to console & knowledge of boundaries
		// some sort of boundary map to determine efficiently when to 
		//Point leftbound;
		int yoff;
		unsigned int len;
		DLinkList<DLinkList<ConsDiff>> lines;

		void BasicEncode(char* encode);
	};
}