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

	char c = cin.CheckInput();
	cout.Write("\033[36mtEsT sTrInG\033[0m\n");
	
	cin.BlockInput();
	
	cout.Write("\033[35mTeSt StRiNg\033[0m\n");
	cout.Write("\033[30moH hEy CoOl, \033[31mNeAt\033[0m\n");
	//fflush(stdout); //absolutely unnecessary for this test, still a useful reference tho.
}
