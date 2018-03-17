#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
    int errnum;

    for(errnum = 0; errnum < 256; ++errnum)
    {
        printf("%3d: %s\n", errnum, strerror(errnum));
    }
    return 0;
}
