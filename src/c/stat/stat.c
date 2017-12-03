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
    int i;
    char* p;
    struct stat buf;
    for(i = 1; i < argc; ++i)
    {
        p = argv[i];
        if(lstat(p, &buf) < 0)
        {
            perror("lstat error");
            continue;
        }
        if(buf.st_mode & S_ISUID)
        {
            printf("IS UID\n");
        }
        if(buf.st_mode & S_ISGID)
        {
            printf("IS GID\n");
        }
        buf.st_mode |= S_ISUID;
    }
    return 0;
}
