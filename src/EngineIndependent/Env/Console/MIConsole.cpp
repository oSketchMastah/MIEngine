#include "MIConsole.h"
#include "MIConsoleInput.h"
#include "MIConsoleOutput.h"
#include <cstdio>

#if defined( WIN32 ) || defined(WIN64)
#include <Windows.h>
#else
#include <unistd.h>
#include <termios.h>
#endif

using namespace MI;
struct Console::Impl {
	Impl();
	~Impl() {}
    ConsoleOutput pout;
    ConsoleInput pin;
};

template<> ConsoleOutput& Console::Get<ConsoleOutput>() { return pImpl->pout; }
template<> ConsoleInput& Console::Get<ConsoleInput>() { return pImpl->pin; }

int Console::Initialize(const int inbufsize, const int outbufsize) {
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
	if (setvbuf(stdout, NULL, _IOFBF, outbufsize) != 0) {
		fprintf(stdout, "\033[31m%s\033[0m\n", "Error setting output buffer");
		return 1;
	}
    if (setvbuf(stdout, NULL, _IOLBF, inbufsize) != 0) {
        fprintf(stdout, "\033[31m%s\033[0m\n", "Error setting input buffer");
        return 1;
    }
}

/*
TerminalString& Console::GetInput() {

}
*/

