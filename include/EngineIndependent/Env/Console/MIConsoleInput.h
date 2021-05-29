#pragma once

struct TerminalString;
namespace MI {
	struct ConsoleInput {
		//put the console in unblocked input mode (characters can be read the moment they are entered)
		void UnblockInput();
		//check the input for a character (for use in unblocked mode)
		char CheckInput();
		//blocks input. (this is the default, but probably should be used sometime after UnblockInput)
		void BlockInput(const int bufsize = 16385);
		TerminalString& GetInput();
	};
}