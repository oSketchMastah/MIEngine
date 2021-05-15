#pragma once

namespace MI {

struct TerminalString;
struct Console {
	//Sets up the console, might include installing our own buffer in the current terminal, setting up virtual sequence interpretation (in windows)
	static int Initialize(const int bufsize = 16385);
	//put the console in unblocked input mode (characters can be read the moment they are entered)
	void UnblockInput();
	//check the input for a character (for use in unblocked mode)
	char CheckInput();
	
	//blocks input. (this is the default, but probably should be used sometime after UnblockInput)
	void BlockInput();
	TerminalString& GetInput();
	
	//Write a TerminalString to the console
	void Write(const TerminalString& tstr);
	template<typename TString, typename ...Rest>
	void out(const TString& tstr, const Rest & ... rest) {
		Write(tstr);
		if constexpr (sizeof...(Rest) > 0) { out(rest...); }
	};
	
	
};

};
