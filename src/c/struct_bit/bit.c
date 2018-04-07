#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "util.h"

typedef struct header{
    unsigned int a:4;
    unsigned int b:8;
    unsigned int c:8;
    unsigned int d:8;
    unsigned int e:4;
}header;

typedef union U{
    unsigned int x;
    header head;
}U;

void dump(U* pU)
{
    printf("x = 0x%08X\n", pU->x);
    printf("a = 0x%X\n", pU->head.a);
    printf("b = 0x%X\n", pU->head.b);
    printf("c = 0x%X\n", pU->head.c);
    printf("d = 0x%X\n", pU->head.d);
    printf("e = 0x%X\n", pU->head.e);
}

int main(int argc, char* argv[])
{
    U u;
    unsigned int v = 0x12345678;
    printf("sizeof(u) = %d\n", (int)sizeof(u));
    memset(&u, 0x00, sizeof(u));
    u.head.a = 1;
    util_hex_dump("head", (uint8_t*)&u, sizeof(u));
    dump(&u);
    return 0;
}
