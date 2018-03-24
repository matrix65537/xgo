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
    unsigned int i = 0x01;
    uint8_t* p;
    printf("sizeof(unsigned int) = %d\n", sizeof(i));
    p = (uint8_t*)&i;
    if(*p == 0x01)
    {
        printf("little endian\n");
    }
    else
    {
        printf("big endian\n");
    }
    return 0;
}
