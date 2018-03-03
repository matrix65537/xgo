#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define BLOCK_SIZE	(1 * 1024 * 1024)

int main(int argc, char* argv[])
{
    unsigned long block_size;
    unsigned long total_size;
    uint8_t* pData;

    block_size = BLOCK_SIZE;

    while(1)
    {
        pData = malloc(block_size);
        if(pData == NULL)
        {
            if(block_size <= 2)
            {
                break;
            }
            else
            {
                printf("block size: %08X\n", block_size);
                block_size >>= 1;
            }
        }
        else
        {
            total_size += block_size;
            //printf(".");
            //printf("total size: %08X\n", total_size);
        }
    }
    printf("total size: %08X\n", total_size);
    return 0;
}
