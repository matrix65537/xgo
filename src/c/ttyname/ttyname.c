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
    char* name;
    for(i = 0; i < 3; ++i)
    {
        name = ttyname(i);
        if(name)
        {
            printf("ttyname%d = %s\n", i, name);
        }
        else
        {
            perror("ttyname");
        }
    }
    return 0;
}
