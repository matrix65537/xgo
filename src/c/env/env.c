#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "util.h"

extern char** environ;

int main(int argc, char* argv[])
{
    char** ep = environ;
    int i = 0;
    while(*ep)
    {
        printf("env%02d: %s\n", i, *ep);
        ep++;
        i++;
    }
    return 0;
}
