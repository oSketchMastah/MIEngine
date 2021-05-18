#pragma once
#include "MIBankString.h"
#include "MIRange.h"
#include "MILinkedList.h"
#include "MIStringUtils.h"

struct CWindow;
namespace MI {

	enum Efx : int {
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

	struct ConsDiff {
		Efx effector;
		Range<char> range;
		char c;
	};
	
	struct Point {
		int x;
		int y;
	};
	void Repeat(char c, int size, char*& encode) {
		for (int i = size; i != 0; i--; encode++) {
			*encode = c;
		}
	}
	void moveTo(int row, int col) {
		fprintf(stdout, "\x1b[%d;%df", row, col);
	}
	//Encode to a high-level diff encoding, for easy construction of a delayered composition
	struct BasicAnimation {
		const CWindow& win; //access to console & knowledge of boundaries
		char* tofill;
		// some sort of boundary map to determine efficiently when to 
		//Point leftbound;
		
		DLinkList<DLinkList<DiffEncode>> lines;
		void Update(char* toDisplay) {
			fillstr.str = toDisplay;
		}

		void BasicEncode(char * encode) {
			for (DLinkList<DLinkList<DiffEncode>>::Node* line : lines) {
				for (DLinkList<DiffEncode>::Node* seq : line->val) {
					auto& [efx, range, c] = seq->val;
					int copytotal = range.last - range.first + 1;
					Repeat(c, copytotal, encode);
				}
			}
		}
	};
}