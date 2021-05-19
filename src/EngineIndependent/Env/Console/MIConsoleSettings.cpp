#include "MIConsoleSettings.h"

#ifndef WINDOWS
#include "unistd.h"
#endif

using namespace MI;
	/*if (vseqinterp) {
		HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
		//DWORD dwMode;
		GetConsoleMode(hOutput, &init_mode);
		curr_mode = init_mode;
		
		curr_mode |= ENABLE_PROCESSED_OUTPUT | ENABLE_VIRTUAL_TERMINAL_PROCESSING;
		if (SetConsoleMode(hOutput, dwMode) == 0) {
			fprintf(stdout, "\033[31m%s\033[0m\n", "SetConsoleMode Failed");
			return 1;
		}
	}*/
void ConsoleSettings::Initialize() {
	
#ifdef WINDOWS
	GetConsoleMode(GetStdHandle(STD_OUTPUT_HANDLE), &init_outmode);
	GetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), &init_inmode);
#else
	tcgetattr(STDIN_FILENO, &init_inmode);
	tcgetattr(STDOUT_FILENO, &init_outmode);
	
	/* disable canonical mode (buffered i/o) and local echo */
	//curr_mode.c_lflag &=(~ICANON );/*& ~ECHOE  );*/
#endif
}
#include <cstdio>
int ConsoleSettings::Restore() {
	int retv = 0; //good
	//if (init_outmode != curr_outmode) {
#ifdef WINDOWS
		retv |= (SetConsoleMode(GetStdHandle(STD_OUTPUT_HANDLE), &init_outmode) == 0) ? -1 : 0;
#else
		retv |= tcsetattr(STDOUT_FILENO, TCSANOW, &init_outmode);
#endif
fprintf(stdout, "%d\n", retv);

#ifdef WINDOWS
		retv |= (SetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), &init_inmode) == 0) ? -1 : 0;
#else
		retv |= tcsetattr(STDIN_FILENO, TCSANOW, &init_inmode);
#endif

	return retv;
}

#ifdef WINDOWS
	int ConsoleSettings::DisjunctInputFlags(__in__ DWORD flags) {
		HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
		GetConsoleMode(hIn, &curr_inmode);
		curr_inmode |= flags;
		return SetConsoleMode(hIn, curr_inmode) - 1;
	}
	
	int ConsoleSettings::ConjunctInputFlags(__in__ DWORD flags) {
		HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
		GetConsoleMode(hIn, &curr_inmode);
		curr_inmode &= flags;
		return SetConsoleMode(hIn, curr_inmode) - 1;
	}
	
	int ConsoleSettings::DisjunctOutputFlags(__in__ DWORD flags) {
		HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
		GetConsoleMode(hOut, &curr_outmode);
		curr_outmode |= flags;
		return SetConsoleMode(hOut, curr_outmode) - 1;
	}
	
	int ConsoleSettings::ConjunctOutputFlags(__in__ DWORD flags) {
		HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
		GetConsoleMode(hOut, &curr_outmode);
		curr_outmode &= flags;
		return SetConsoleMode(hOut, curr_outmode) - 1//==-1 -> fail
	}
#else
	int ConsoleSettings::DisjunctInputFlags(tcflag_t flags) {
		tcgetattr(STDIN_FILENO, &curr_inmode);
		curr_inmode.c_lflag |= flags;
		return tcsetattr(STDIN_FILENO, TCSANOW, &curr_inmode);
	}
	int ConsoleSettings::ConjunctInputFlags(tcflag_t flags) {
		tcgetattr(STDIN_FILENO, &curr_inmode);
		curr_inmode.c_lflag &= flags;
		return tcsetattr(STDIN_FILENO, TCSANOW, &curr_inmode);
	}
	
	int ConsoleSettings::DisjunctOutputFlags(tcflag_t flags) {
		tcgetattr(STDOUT_FILENO, &curr_outmode);
		curr_outmode.c_lflag |= flags;
		return tcsetattr(STDOUT_FILENO, TCSANOW, &curr_outmode);
	}
	int ConsoleSettings::ConjunctOutputFlags(tcflag_t flags) {
		tcgetattr(STDOUT_FILENO, &curr_outmode);
		curr_outmode.c_lflag &= flags; //(~ICANON );/*& ~ECHOE  );*/
		return tcsetattr(STDOUT_FILENO, TCSANOW, &curr_outmode);
	}
#endif
