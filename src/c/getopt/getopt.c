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
    int ret;
    int i;
    uint8_t buf[0x1000];
    extern char *optarg;
    extern int optind, opterr, optopt;
    const char* optstring = "abc:d:e::fg:";
    for(i = 0; i < argc; ++i)
    {
        printf("%2d: %s\n", i, argv[i]);
    }
    printf("optstring: %s\n", optstring);
    while(1)
    {
        ret = getopt(argc, argv, optstring);
        if(ret == -1)
        {
            break;
        }
        printf("ret = %c\n", (char)ret);
        printf("optarg = %s\n", optarg);

        printf("optind = %d\n", optind);
        printf("opterr = %d\n", opterr);
        printf("optopt = %d\n", optopt);
        printf("====================\n");
    }
    return 0;
}
