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
    int p = 5;
    printf("sizeof(char) = %d\n", (int)sizeof(char));
    printf("sizeof(short) = %d\n", (int)sizeof(short));
    printf("sizeof(int) = %d\n", (int)sizeof(int));
    printf("sizeof(long) = %d\n", (int)sizeof(long));
    printf("sizeof(long long) = %d\n", (int)sizeof(long long));
    printf("sizeof(*) = %d\n", (int)sizeof(&p));
    printf("sizeof(size_t) = %d\n", (int)sizeof(size_t));
    printf("sizeof(ssize_t) = %d\n", (int)sizeof(ssize_t));
    return 0;
}
