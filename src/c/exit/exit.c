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
    int v, v2;
    if(argc < 2)
    {
        printf("Usage: %s exit_status\n", argv[0]);
        exit(1);
    }
    v = atoi(argv[1]);
    v2 = (((unsigned int)v) & 0377);
    printf("exit status = %d\n", v);
    printf("exit status = %d\n", v2);
    exit(v);
    return 0;
}
