#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/utsname.h>
#include "util.h"

int main(int argc, char* argv[])
{
    struct utsname uts;
    int ret;

    ret = uname(&uts);
    if(ret == -1)
    {
        perror("uname error");
        exit(1);
    }
    printf(" sysname: %s\n", uts.sysname);
    printf("nodename: %s\n", uts.nodename);
    printf(" release: %s\n", uts.release);
    printf(" version: %s\n", uts.version);
    printf(" machine: %s\n", uts.machine);
    return 0;
}

