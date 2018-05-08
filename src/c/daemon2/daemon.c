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

void go_daemon(void)
{
    pid_t pid;

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
}

int main(int argc, char* argv[])
{

    go_daemon();
    while(1)
    {
        sleep(1);
    }
    return 0;
}
