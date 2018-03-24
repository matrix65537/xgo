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
    int status;
    pid_t pid;
    printf("before wait\n");
    pid = wait(&status);
    if(pid == -1)
    {
        perror("wait error");
    }
    else
    {
        printf("after wait, pid = %d\n", pid);
    }
    return 0;
}
