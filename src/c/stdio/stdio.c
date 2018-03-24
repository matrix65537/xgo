#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "util.h"

#define BLOCK_SIZE  0x20
int main(int argc, char* argv[])
{
    int i;
    char* pbuf;
    int ret;
    FILE* file;
    uint8_t data[] = {0x01, 0x02, 0x03, 0x04};
    size_t wsize;

    if(argc < 2)
    {
        printf("Usage: %s filename\n", argv[0]);
        exit(1);
    }

    pbuf = malloc(BLOCK_SIZE);
    file = fopen(argv[1], "wb+");
    if(file == NULL)
    {
        perror("fopen error");
        exit(1);
    }

    ret = setvbuf(file, pbuf, _IOFBF, BLOCK_SIZE);
    if(ret != 0)
    {
        perror("setvbuf error");
        exit(1);
    }
    for(i = 0; i < 2; ++i)
    {
        memset(data, (uint8_t)(i+1), sizeof(data));
        wsize = fwrite(data, sizeof(data), 1, file);
        if(wsize == -1)
        {
            perror("fwrite error");
            exit(1);
        }
        else
        {
            util_hex_dump("buf", pbuf, BLOCK_SIZE);
            printf("fwrite return %d\n", wsize);
        }
    }
    return 0;
}

