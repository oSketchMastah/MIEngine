#include "MIConsoleOutput.h"
#include "MIStringUtils.h"
#include <stdio.h>

using namespace MI;
void ConsoleOutput::Write(const char* str) {
    //fprintf(stdout, "%d\n", MI::strlen(str));
    fprintf(stdout, "%s", str);
    //fwrite(str, 1, MI::strlen(str) + 1, stdout);
}

