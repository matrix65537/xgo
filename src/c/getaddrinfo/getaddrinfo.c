#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include "util.h"

int main(int argc, char* argv[])
{
    struct addrinfo hints; 
    struct addrinfo* result;
    struct addrinfo* rp;
    int s;

    if(argc != 3)
    {
        fprintf(stderr, "Usage: %s host port\n", argv[0]);
        exit(1);
    }

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = 0;
    hints.ai_protocol = 0;

    s = getaddrinfo(argv[1], argv[2], &hints, &result);
    if(s != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
        exit(1);
    }

    for(rp = result; rp != NULL; rp = rp->ai_next)
    {
        printf("(%2d, %2d, %2d)\n", rp->ai_family, rp->ai_socktype, rp->ai_protocol);
    }
    freeaddrinfo(result);

    return 0;
}

