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
    pid_t pid;
    pid_t ppid;

    pid = getpid();
    ppid = getppid();

    printf(" PID: %ld\n", (long)pid);
    printf("PPID: %ld\n", (long)ppid);
    return 0;
}
