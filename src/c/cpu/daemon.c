#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <fcntl.h>
#include <unistd.h>
#include "util.h"
#include "time.h"

int main(int argc, char* argv[])
{
    pid_t pid;
    int fd;
    int ret;
    char* timeinfo;

    if(argc < 2)
    {
        printf("Usage %s filename\n", argv[1]);
        exit(1);
    }

    pid = fork();
    if(pid < 0)
    {
        perror("fork error");
        exit(1);
    }
    else if(pid != 0)
    {
        exit(0);
    }
    fd = open(argv[1], O_RDWR | O_CREAT, 0755);
    if(fd == -1)
    {
        perror("open error");
        exit(1);
    }

    setsid();

    if(chdir("/") < 0)
    {
        perror("chdir error");
        exit(1);
    }
    close(0);
    open("/dev/null", O_RDWR);
    dup2(0, 1);
    dup2(0, 2);

    while(1)
    {
    }
    return 0;
}
