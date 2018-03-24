#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <fcntl.h>
#include <unistd.h>
#include "util.h"
#include "time.h"

int main(int argc, char* argv[])
{
    struct timeval tv;
    int ret;
    time_t t;

    ret = gettimeofday(&tv, NULL);
    if(ret == -1)
    {
        perror("gettimeofday error");
        exit(1);
    }
    printf("      second: %d\n", (int)tv.tv_sec);
    printf("micro second: %d\n", (int)tv.tv_usec);
    t = time(NULL);
    printf("        time: %d\n", (int)t);
    printf("%s\n", ctime(&t));

    printf("   gmtime: %s", asctime(gmtime(&t)));
    printf("localtime: %s", asctime(localtime(&t)));


    return 0;
}
