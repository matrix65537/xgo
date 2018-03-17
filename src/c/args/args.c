#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "util.h"

int main(int argc, char* argv[])
{
    int i;
    for(i = 0; i < argc; ++i)
    {
        printf("%02d: %s\n", i, argv[i]);
    }
    return 0;
}
