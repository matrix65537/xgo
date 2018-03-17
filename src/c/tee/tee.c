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
    int fd;
    uint8_t buf[0x1000];
    if(argc < 2)
    {
        printf("usage: %s filename\n", argv[0]);
        exit(1);
    }
    //open file
    fd = open(argv[1], O_RDWR | O_CREAT | O_TRUNC, 0);
    if(fd == -1)
    {
        perror("open error");
        exit(1);
    }

    while(1)
    {
        //read stdin
        ret = read(STDIN_FILENO, buf, sizeof(buf));
        if(ret == -1)
        {
            perror("read error");
            exit(1);
        }
        if(ret == 0)
        {
            break;
        }
        //write file
        ret = write(fd, buf, ret);
        if(ret == -1)
        {
            perror("write file error");
            exit(2);
        }
        //write stdout
        ret = write(STDOUT_FILENO, buf, ret);
        if(ret == -1)
        {
            perror("write stdout error");
            exit(2);
        }
    }
    return 0;
}
