#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define MAX_FILE_COUNT  (16 * 1024)

#define BLOCK_SIZE	(256 * 1024 * 1024)

int main(int argc, char* argv[])
{
    int fd;
    int ret;
    int i;
    char buf[0x100];
    off_t offset;
    uint8_t* pData;
    
    pData = malloc(BLOCK_SIZE);
    memset(pData, 'A', BLOCK_SIZE);
    if(pData == NULL)
    {
	exit(1);
    }
    for(i = 0; i < MAX_FILE_COUNT; ++i)
    {
        snprintf(buf, sizeof(buf), "%08X.c", i);
        printf("%s\n", buf);
        fd = open(buf, O_RDWR | O_CREAT);
        if(fd == -1) {
            perror(argv[0]);
            break;
        }

	/*
	offset = lseek(fd, (1024 * 1024), SEEK_CUR); 
        if(offset == -1) {
            perror(argv[0]);
            break;
        }
	*/
	ret = write(fd, pData, BLOCK_SIZE);
        if(ret == -1)
        {
            perror(argv[0]);
            break;
        }

    	ret = close(fd);
        if(ret == -1)
        {
            perror(argv[0]);
            break;
        }
    }
    return 0;
}
