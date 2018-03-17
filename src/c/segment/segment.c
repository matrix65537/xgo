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
    void* p;
    int i;
    extern char etext, edata, end;
    printf("etext: %p\n", &etext);
    printf("edata: %p\n", &edata);
    printf("  end: %p\n", &end);

    for(i = 1; i < 4096; ++i)
    {
        p = sbrk(i);
        if(p == (void*)-1)
        {
            perror("sbrk error");
            exit(1);
        }
        *((char*)p) = 0x0A;
        printf("0x%04X : %p\n", i, p);
    }
    p = malloc(0x04);
    printf("%p\n", p);
    p = malloc(0x04);
    printf("%p\n", p);
    p = malloc(0x04);
    printf("%p\n", p);
    return 0;
}
