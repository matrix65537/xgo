#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
    int fd;
    int ret;
    char buf[0x100];
    memset(buf, 'A', sizeof(buf));
    
    fd = open(argv[1], O_RDWR | O_CREAT);
    if(fd == -1)
    {
        perror("open error");
        exit(1);
    }

    ret = write(fd, buf, 0x10);
    if(ret == -1)
    {
        perror("write error");
        exit(1);
    }
    ret = read(fd, buf, 0x10);
    if(ret == -1)
    {
        perror("read error");
        exit(1);
    }

    ret = close(fd);
    if(ret == -1)
    {
        perror("close error");
        exit(1);
    }
    return 0;
}
