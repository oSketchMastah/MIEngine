#include "MIFileMode.h"
#include "MIStringUtils.h"
#include "MIAssert.h"

using namespace MI;
	//this gets a file open mode string from a bunch of bitflags, Ex. F
FMode::FMode(const FMode& c) {
	strcpy(modeStr, 4, c.modeStr);
}

FMode::FMode(const char c) noexcept {
	switch (c & (in | out)) { //0, or some combination of in and out
	case in:
		modeStr[0] = 'r'; break;
	case out:
		if (c & app) {
			modeStr[0] = 'a'; break;
		}
		else if (c & ovw) {
			modeStr[0] = 'w'; break;
		}
		else {
			modeStr[0] = 'r'; modeStr[1] = '+'; break;
		}
	case in | out:
		modeStr[0] = 'r'; modeStr[1] = '+'; break;
	default://0
		DoAssertMsg("bad file mode");
	}

	switch (c & (app | ovw)) { //0, or some combination of in and out
	case 0:
		break;
	case app: // swap first (append takes precedence over read/write)
		if (c == in) { modeStr[1] = '+'; }
		modeStr[0] = 'a'; break;
	case ovw: //
		modeStr[0] = 'w'; break;
	default://0
		DoAssertMsg("bad file mode");
	}

	switch (c & txt) {
	case 0:
		modeStr[2] = 'b'; break;
	default:
		modeStr[2] = 't'; break;
	}
}