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
    char* p;
    uint8_t buf[0x100];
    while(1)
    {
        printf(": ");
        p = fgets(buf, sizeof(buf), stdin);
        if(p == NULL)
        {
            break;
        }
        util_hex_dump("buf", buf, sizeof(buf));
        printf("%d: %s\n", (int)strlen(buf), buf);
    }
    return 0;
}
