#include "MIConsole.h"
#include "MIConsoleInput.h"
#include "MIConsoleOutput.h"
#include "MIConsoleSettings.h"
#include <cstdio>

#if defined( WIN32 ) || defined(WIN64)
#include <Windows.h>
#endif

using namespace MI;
struct Console::Impl {
	Impl();
	~Impl() {}
	
	ConsoleOutput Out;
	ConsoleInput In;
};

template<> ConsoleOutput& Console::Get<ConsoleOutput>() { return pImpl->Out; }
template<> ConsoleInput& Console::Get<ConsoleInput>() { return pImpl->In; }
//template<> ConsoleSettings& Console::Get<ConsoleSettings>() { return pImpl->Settings; }

Console::~Console() {
	/*if (ConsoleSettings::Restore() != 0) { 
		fprintf(stdout, "\033[31m%s\033[0m\n", "Failed to restore terminal settings");
	}*/
}

int Console::Initialize(const int inbufsize, const int outbufsize) {
	if (outbufsize != -1 && setvbuf(stdout, NULL, _IOFBF, outbufsize) != 0) {
		fprintf(stdout, "\033[31m%s\033[0m\n", "Error setting output buffer");
		return 1;
	}
    	if (inbufsize != -1 && setvbuf(stdin, NULL, _IOLBF, inbufsize) != 0) {
        	fprintf(stdout, "\033[31m%s\033[0m\n", "Error setting input buffer");
       	 return 1;
   	}
   	
//setup virtual sequence interpretation stuff on windows consoles where it gets disabled
#if defined( WIN32 ) || defined(WIN64)
	if (ConsoleSettings::DisjunctOutputFlags(ENABLE_PROCESSED_OUTPUT | ENABLE_VIRTUAL_TERMINAL_PROCESSING) != 0) {
		fprintf(stdout, "\033[31m%s\033[0m\n", "SetConsoleMode failed on adding flags ENABLE_PROCESSED_OUTPUT, and ENABLE_VIRTUAL_TERMINAL_PROCESSING");
		return 1;
	}
#endif
}

/*
TerminalString& Console::GetInput() {

}
*/

