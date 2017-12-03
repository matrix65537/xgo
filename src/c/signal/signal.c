#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include "util.h"

static void sig_int(int);

typedef void (*sighandler_t)(int);

int main(int argc, char* argv[])
{
    int ret;
    int i;
    uint8_t buf[0x1000];
    sighandler_t sig_handle;

    for(i = 0; i < 256; ++i)
    {
        sig_handle = signal(i, sig_int);
        if(sig_handle == SIG_ERR)
        {
            snprintf(buf, sizeof(buf), "add signal [%d] error", i);
            perror(buf);
        }
    }

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

void sig_int(int signo)
{
    printf("interrupt no: %d\n", signo);
}
