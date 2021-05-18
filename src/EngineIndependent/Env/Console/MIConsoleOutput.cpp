#include "MIConsoleOutput.h"
#include "MIStringUtils.h"
#include <stdio.h>

using namespace MI;
void ConsoleOutput::Write(const char* str) {
    fwrite(str, 1, strlen(str) + 1, stdout);
}

