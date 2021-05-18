#include "MIConsole.h"
#include "MIConsoleInput.h"
#include "MIConsoleOutput.h"
#include <stdio.h>
using namespace MI;
int main() {
	Console::Initialize();
	Console cons;
	ConsoleInput& cin = cons.Get<ConsoleInput>();
	ConsoleOutput& cout = cons.Get<ConsoleOutput>();
	cin.UnblockInput();
	char c;


	for (int i = -1; (c = cin.CheckInput()) == '\0'; i = (i + 1)%100000) {
		if (i == 0) {
			cout.Write("hit a key\n");
			fflush(stdout);
		}
	}


	
	fprintf(stdout, "you hit %c\n", c);
	cin.UnblockInput();
}