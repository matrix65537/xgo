#define _XOPEN_SOURCE
#include <unistd.h>

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include "util.h"

int main(int argc, char* argv[])
{
    char *r;
    int i;
    if(argc < 3)
    {
        printf("Usage: %s key salt\n", argv[0]);
        exit(1);
    }
    for(i = 0; i < argc; ++i)
    {
        printf("%2d: %s\n", i, argv[i]);
    }

    errno = 0;
    r = crypt(argv[1], argv[2]);
    if(r != NULL)
    {
        printf("result:%s\n", r);
    }
    else
    {
        perror("crypt error");
    }
    return 0;
}
