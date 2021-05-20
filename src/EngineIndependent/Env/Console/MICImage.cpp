#pragma once
#include "MICImage.h"
#include "MIStringUtils.h"

using namespace MI;
void Repeat(char c, int size, char*& encode) {
	for (int i = size; i > 0; i--; encode++) {
		*encode = c;
	}
}
void EncodeString(int v, char*& encode) {
	unsigned int workingv;
	if (v >= 0) {
		workingv = v;
	}
	else {
		*(encode++) = '-';
		workingv = abs(v);
	}

	for (; workingv != 0; workingv /= 10) {
		*(encode++) = '0' + v % 10;
	}
}
void moveTo(int row, int col, char*& encode) {
	*(encode++) = '\x1b';
	*(encode++) = '[';
	EncodeString(row, encode);
	*(encode++) = ';';
	EncodeString(col, encode);
	*(encode++) = 'f';
}

void Colorize(ColorEfx efx, char*& encode) {
	if (efx == 0) { return; }//0 is default, we take this to mean (do nothing)
	else if (efx == -1) { efx == 0; } //-1 is 'Reset', done with \033[0m.

	*(encode++) = '\033';
	*(encode++) = '[';
	EncodeString(static_cast<int>(efx), encode);
	*(encode++) = 'm';
}

void Image::BasicEncode(char* encode) {
	int row = yoff;
	for (DLinkList<DLinkList<ConsDiff>>::Node* line : lines) {
		int lcol = 1 << 31; //just initializing this to something really big
		int fcol;
		for (DLinkList<ConsDiff>::Node* seq : line->val) {
			auto& [efx, range, c] = seq->val;
			fcol = range.first;

			if (fcol > lcol + 1) {
				moveTo(row, fcol, encode);
			}

			lcol = range.last;
			Colorize(efx, encode);
			Repeat(c, range.last - range.first + 1, encode);
		}
		row++;
	}
}

}