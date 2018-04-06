#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <pcap.h>
#include <time.h>
#include "util.h"

void capture_packet1(pcap_t* device)
{
    struct pcap_pkthdr pkthdr;
    char errbuf[PCAP_ERRBUF_SIZE];
    const u_char* pkt = pcap_next(device, &pkthdr);
    if(!pkt)
    {
        printf("couldn't capture packet: %s\n", errbuf);
        exit(1);
    }
    printf(" Packet length: %d\n", pkthdr.len);
    printf("Capture length: %d\n", pkthdr.caplen);
    printf(" Recieved time: %s\n", ctime((const time_t*)&pkthdr.ts.tv_sec));
}

void getPacket(u_char* arg, const struct pcap_pkthdr* pkthdr, const u_char* packet)
{
    int* id = (int*)arg;
    printf("            id: %d\n", *id); 
    *id += 1;
    printf(" Packet length: %d\n", pkthdr->len);
    printf("Capture length: %d\n", pkthdr->caplen);
    printf(" Recieved time: %s\n", ctime((const time_t*)&(pkthdr->ts.tv_sec)));
    util_hex_dump("net data", (uint8_t*)packet, pkthdr->caplen);
}

void capture_packet2(pcap_t* device)
{
    struct pcap_pkthdr packet;
    int id = 0;
    pcap_loop(device, -1, getPacket, (u_char*)&id);
}

int main(int argc, char* argv[])
{
    char errbuf[PCAP_ERRBUF_SIZE];
    char* dev;

    struct in_addr addr;
    char* net;
    char* mask;
    bpf_u_int32 netp, maskp;
    int err;

    pcap_t* device;
    struct bpf_program filter;

    if(argc != 2)
    {
        //printf("Usage: %s filter\n", argv[0]);
        //exit(1);
    }

    dev = pcap_lookupdev(errbuf);
    if(dev)
    {
        printf("dev: %s\n", dev);
    }
    else
    {
        printf("error: %s\n", errbuf);
    }

    err = pcap_lookupnet(dev, &netp, &maskp, errbuf);
    if(err == -1)
    {
        printf("couldn't detect the ip and maskp: %s\n", errbuf);
        exit(1);
    }

    addr.s_addr = netp;
    net = inet_ntoa(addr);
    if(net)
    {
        printf("  ip: %s\n", net);
    }
    addr.s_addr = maskp;
    mask = inet_ntoa(addr);
    if(mask)
    {
        printf("mask: %s\n", mask);
    }

    device = pcap_open_live(dev, 65535, 1, 0, errbuf);
    if(!device)
    {
        printf("couldn't open the net device: %s\n", errbuf);
        exit(1);
    }

    if(argc == 2)
    {
        printf("filter = %s\n", argv[1]);
        err = pcap_compile(device, &filter, argv[1], 1, 0);
        if(err != 0)
        {
            printf("%s\n", pcap_geterr(device));
            exit(1);
        }
        pcap_setfilter(device, &filter);
    }
    capture_packet2(device);
    exit(1);

    while(1)
    {
        capture_packet1(device);
    }

    return 0;
}
