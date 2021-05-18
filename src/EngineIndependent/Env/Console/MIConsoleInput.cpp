#include "MIConsoleInput.h"
#include <stdio.h>

#if defined( WIN32 ) || defined(WIN64)
#include <Windows.h>
#include <conio.h>
#endif

using namespace MI;
//put the console in unblocked input mode (characters can be read the moment they are entered)
void ConsoleInput::UnblockInput() {
    if (setvbuf(stdin, NULL, _IONBF, 0) != 0) {
        fprintf(stdout, "\033[31m%s\033[0m\n", "error removing buffer on input");
    }
#if defined(WIN32) || defined(WIN64)
#else
    fcntl(stdin, F_SETFL, fcntl(stdin, F_GETFL, 0) | O_NONBLOCK);
#endif
}

//check the input for a character (for use in unblocked mode)
char ConsoleInput::CheckInput() {
    char c = '\0';
#if defined(WIN32) || defined(WIN64)
    if (_kbhit()) {
        return _getch();
    }
    //fread(&c, 1, 1, stdin);
#else
    read(0, &c, 1);
#endif
    return c;
}

//blocks input. (this is the default, but probably should be used sometime after UnblockInput)
void ConsoleInput::BlockInput(const int bufsize) {
    if (setvbuf(stdin, NULL, _IOLBF, bufsize) != 0) {
        fprintf(stdout, "\033[31m%s\033[0m\n", "Error reallocating input buffer");
    }
#if defined(WIN32) || defined(WIN64)
    //for windows, it seems easier to change how we handle input (kbhit and getchar)
#else
    fcntl(stdin, F_SETFL, fcntl(stdin, F_GETFL, 0) & ~O_NONBLOCK);
#endif
}