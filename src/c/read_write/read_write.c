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
    int ret;
    uint8_t buf[0x1000];
    while(1)
    {
        ret = read(STDIN_FILENO, buf, sizeof(buf));
        if(ret == -1)
        {
            perror("read error");
            exit(1);
        }
        if(ret == 0)
        {
            break;
        }
        ret = write(STDOUT_FILENO, buf, ret);
        if(ret == -1)
        {
            perror("write error");
            exit(2);
        }
    }
    return 0;
}
