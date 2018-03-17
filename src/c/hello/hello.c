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
    char buf[0x10];
    memset(buf, 'A', sizeof(buf));
    printf(" uid: %d\n", getuid());
    printf("euid: %d\n", geteuid());
    printf(" gid: %d\n", getgid());
    printf("egid: %d\n", getegid());
    fd = open("./a.txt", O_RDWR | O_CREAT | O_TRUNC, 0755);
    if(fd == -1)
    {
        perror("open error");
    }
    ret = write(fd, buf, sizeof(buf));
    if(fd == -1)
    {
        perror("write error");
    }
    return 0;
}
