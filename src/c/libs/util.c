#include <stdio.h>
#include <ctype.h>
#include "util.h"

void util_hex_dump(char* name, uint8_t* pData, uint32_t len)
{
    int i;
    int j;
    int col = 0x10;
    int half_col = col >> 1;
    int row;
    int remain;
    uint8_t v;
    printf("################################################################################\n");
    if(name)
    {
        printf("%s[len = 0x%04X, %4d bytes]\n", name, len, len);
    }
    row = len / col;
    remain = len % col;
    for(i = 0; i < row; ++i)
    {
        for(j = i * col; j < (i + 1) * col; ++j)
        {
            printf("%02X ", pData[j]);
            if((j % col ) == (half_col - 1))
            {
                printf("  ");
            }
        }
        printf("    ");
        for(j = i * col; j < (i + 1) * col; ++j)
        {
            v = pData[j];
            if((v == 0x0D) || (v == 0x0A) || !isgraph(v))
            {
                v = '.';
            }
            printf("%c", v);
        }
        printf("\n");
    }
    j = row * col;
    while(j < (row + 1) * col)
    {
        if(j < len)
        {
            printf("%02X ", pData[j]);
        }
        else
        {
            printf("   ");
        }
        if((j % col ) == (half_col - 1))
        {
            printf("  ");
        }
        j++;
    }
    printf("    ");
    j = row * col;
    while(j < len)
    {
        v = pData[j++];
        if((v == 0x0D) || (v == 0x0A) || !isgraph(v))
        {
            v = '.';
        }
        printf("%c", v);
    }
    printf("\n");

    //printf("################################################################################\n");
}


void test_hex_dump()
{
    int i;
    char buf[0x100];
    for(i = 0; i < 0x100; ++i)
    {
        buf[i] = i;
    }
    util_hex_dump("buf", buf, 0x0F);
    util_hex_dump("buf", buf, 0xF7);
    util_hex_dump("buf", buf, 0xFF);
    util_hex_dump("buf", buf, 0x100);
}

#if DEBUG_UTIL
int main()
{
    test_hex_dump();
    return 0;
}
#endif
