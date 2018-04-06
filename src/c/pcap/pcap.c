#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <pcap.h>
#include "util.h"

int main(int argc, char* argv[])
{
    char errbuf[PCAP_ERRBUF_SIZE];
    char* device;

    device = pcap_lookupdev(errbuf);
    if(device)
    {
        printf("device: %s\n", device);
    }
    else
    {
        printf("error: %s\n", errbuf);
    }
    return 0;
}
