#include "MIConsole.h"
#include <cstdio>

#if defined( WIN32 ) || defined(WIN64)
#include <Windows.h>
#endif


using namespace MI;

int Console::Initialize(const int bufsize) {
//setup virtual sequence interpretation stuff on windows consoles where it gets disabled
#if defined( WIN32 ) || defined(WIN64)
	HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD dwMode;
	GetConsoleMode(hOutput, &dwMode);
	dwMode |= ENABLE_PROCESSED_OUTPUT | ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	if (SetConsoleMode(hOutput, dwMode) == 0) {
		fprintf(stdout, "\033[31m%s\033[0m\n", "SetConsoleMode Failed");
		return 1;
	}
#endif
	if (setvbuf(stdout, NULL, _IOFBF, bufsize) != 0) {
		fprintf(stdout, "\033[31m%s\033[0m\n", "Error setting buffer");
		return 1;
	}
}

//put the console in unblocked input mode (characters can be read the moment they are entered)
void Console::UnblockInput() {

}

//check the input for a character (for use in unblocked mode)
char Console::CheckInput() {

}
	
//blocks input. (this is the default, but probably should be used sometime after UnblockInput)
void Console::BlockInput() {

}


TerminalString& Console::GetInput() {

}
	
//Write a TerminalString to the console
void Console::Write(const TerminalString& tstr) {

}

