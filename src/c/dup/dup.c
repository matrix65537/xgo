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
    int fd;
    int ret;
    if(argc < 2)
    {
        printf("Usage: %s filename\n", argv[0]);
        exit(1);
    }
    fd = open(argv[1], O_RDWR | O_CREAT, 0755);
    if(fd == -1)
    {
        perror("open error");
        exit(1);
    }
    ret = close(STDOUT_FILENO);
    if(ret == -1)
    {
        perror("close error");
        exit(1);
    }
    fd = dup(fd);
    if(ret == -1)
    {
        perror("dup error");
        exit(1);
    }
    printf("ABCD\n");

    return 0;
}

