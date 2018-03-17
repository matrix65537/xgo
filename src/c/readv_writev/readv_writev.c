#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/uio.h>
#include <fcntl.h>
#include <unistd.h>
#include "util.h"

int main(int argc, char* argv[])
{
    int ret;
    int sum = 0;
    uint8_t buf0[0x20];
    uint8_t buf1[0x10];
    uint8_t buf2[0x08];
    struct iovec iov_vec[0x03];

    iov_vec[0].iov_base = buf0;
    iov_vec[0].iov_len = sizeof(buf0);

    iov_vec[1].iov_base = buf1;
    iov_vec[1].iov_len = sizeof(buf1);

    iov_vec[2].iov_base = buf2;
    iov_vec[2].iov_len = sizeof(buf2);

    sum = iov_vec[0].iov_len + iov_vec[1].iov_len + iov_vec[2].iov_len;

    while(1)
    {
        ret = readv(STDIN_FILENO, iov_vec, 3);
        if(ret == -1)
        {
            perror("readv error");
            exit(1);
        }
        if(ret == 0)
        {
            break;
        }
        printf("ret = %d\n", ret);
        util_hex_dump("vec[0]", iov_vec[0].iov_base, iov_vec[0].iov_len);
        util_hex_dump("vec[1]", iov_vec[1].iov_base, iov_vec[1].iov_len);
        util_hex_dump("vec[2]", iov_vec[2].iov_base, iov_vec[2].iov_len);

    }
    return 0;
}
