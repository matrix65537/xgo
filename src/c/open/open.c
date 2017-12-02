#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define MAX_FILE_COUNT  0x04

int main(int argc, char* argv[])
{
    int fd;
    int ret;
    int i;
    char buf[0x100];

    for(i = 0; i < MAX_FILE_COUNT; ++i)
    {
        snprintf(buf, sizeof(buf), "%08X", i);
        printf("%s\n", buf);
        fd = open(buf, O_RDWR | O_CREAT);
        if(fd == -1)
        {
            perror(argv[0]);
            break;
        }
        ret = write(fd, buf, strlen(buf));
        if(ret == -1)
        {
            perror(argv[0]);
            ret = close(fd);
            if(ret == -1)
            {
                perror(argv[0]);
                break;
            }
        }
        if(ret == -1)
        {
            perror(argv[0]);
            break;
        }
    }
    return 0;
}
