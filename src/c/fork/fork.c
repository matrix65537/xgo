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
    pid_t pid;
    int i;
    for(i = 0; i < argc; ++i)
    {
        printf("%02d: %s\n", i, argv[i]);
    }
    if(argc < 3)
    {
        printf("Usage: %s path argv[]\n", argv[0]);
        exit(1);
    }
    pid = fork();
    if(pid == -1)
    {
        perror("fork error");
        exit(1);
    }
    else if(pid == 0)
    {
        char *const envp[] = {
            "A=1",
            "B=2",
            "C=/home/laoguo",
            "D=hello",
            NULL,
        };
        execve(argv[1], &argv[2], envp);
    }
    else
    {
        int status;
        wait(&status);
    }
    return 0;
}
