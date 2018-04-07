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

typedef struct eth_header{
    uint8_t dest_mac[0x06];
    uint8_t src_mac[0x06];
    uint16_t etype;
}eth_header;

typedef struct ip_header{
    uint8_t header_len:4;
    uint8_t version:4;
    uint8_t service_type;
    uint16_t total_len;
    uint16_t id;
    uint16_t offset;
    uint8_t ttl;
    uint8_t protocol;
    uint16_t check_sum;
    struct in_addr src_ip;
    struct in_addr dest_ip;
}ip_header;

typedef struct tcp_header{
    uint16_t src_port;
    uint16_t dest_port;
    uint32_t seq_no;
    uint32_t ack_no;

    uint8_t reserved_1:4;
    uint8_t header_len:4;
    uint8_t flag:6;
    uint8_t reserved_2:2;
    uint16_t windows_size;
    uint16_t check_sum;
    uint16_t p;
}tcp_header;

typedef struct icmp_header{
    uint8_t icmp_type;
    uint8_t code;
    uint16_t check_sum;
}icmp_header;

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
    eth_header* peth_head;
    ip_header* pip_head;
    tcp_header* ptcp_head;
    uint16_t etype;
    int* id = (int*)arg;
    printf("################################################################################\n");
    printf("            id: %d\n", *id);
    *id += 1;
    printf(" Packet length: %d\n", pkthdr->len);
    printf("Capture length: %d\n", pkthdr->caplen);
    printf(" Recieved time: %s\n", ctime((const time_t*)&(pkthdr->ts.tv_sec)));
    util_hex_dump("network data", (uint8_t*)packet, pkthdr->caplen);

    peth_head = (eth_header*)packet;
    printf("Dest MAC: %02X:%02X:%02X:%02X:%02X:%02X\n",
            peth_head->dest_mac[0],
            peth_head->dest_mac[1],
            peth_head->dest_mac[2],
            peth_head->dest_mac[3],
            peth_head->dest_mac[4],
            peth_head->dest_mac[5]
            );
    printf(" Src MAC: %02X:%02X:%02X:%02X:%02X:%02X\n",
            peth_head->src_mac[0],
            peth_head->src_mac[1],
            peth_head->src_mac[2],
            peth_head->src_mac[3],
            peth_head->src_mac[4],
            peth_head->src_mac[5]
            );
    etype = peth_head->etype;
    etype = ntohs(etype);
    printf("   etype: 0x%04X\n", etype);
    switch(etype)
    {
        case 0x0800:
            {
                printf("====> IP:\n");
                pip_head = (ip_header*)(packet + sizeof(eth_header));
                //util_hex_dump("ip header", (uint8_t*)pip_head, sizeof(ip_header));
                printf("  ip header len: %d bytes\n", pip_head->header_len * 4);
                printf("     ip version: 0x%02X \n", pip_head->version);
                printf("ip service type: 0x%02X \n", pip_head->service_type);
                printf("   ip total len: 0x%04X \n", pip_head->total_len);
                printf("          ip id: 0x%04X \n", pip_head->id);
                printf("      ip offset: 0x%04X \n", pip_head->offset);
                printf("         ip ttl: 0x%02X \n", pip_head->ttl);
                printf("    ip protocol: 0x%02X \n", pip_head->protocol);
                printf("   ip check sum: 0x%04X \n", pip_head->check_sum);
                printf("         src ip: %s\n", inet_ntoa(pip_head->src_ip));
                printf("        dest ip: %s\n", inet_ntoa(pip_head->dest_ip));

                switch(pip_head->protocol)
                {
                    case 0x01:
                        {
                            printf("====> ICMP:\n");
                        }break;
                    case 0x02:
                        {
                            printf("====> IGMP:\n");
                        }break;
                    case 0x06:
                        {
                            int remain_len;
                            printf("====> TCP:\n");
                            ptcp_head = (tcp_header*)(packet + sizeof(eth_header) + sizeof(ip_header));
                            //util_hex_dump("tcp header", (uint8_t*)ptcp_head, sizeof(tcp_header));
                            printf("    src port: %d\n", ntohs(ptcp_head->src_port));
                            printf("   dest port: %d\n", ntohs(ptcp_head->dest_port));
                            printf("      seq no: 0x%08X\n", ptcp_head->seq_no);
                            printf("      ack no: 0x%08X\n", ptcp_head->ack_no);
                            printf("tcp head len: %d\n", ptcp_head->header_len * 4);
                            printf(" window size: %d\n", ptcp_head->windows_size);
                            printf("   check sum: 0x%04X\n", ptcp_head->check_sum);
                            printf("     pointer: 0x%04X \n", ptcp_head->p);
                            remain_len = (pkthdr->caplen - sizeof(eth_header) - pip_head->header_len * 4 - ptcp_head->header_len * 4);
                            if(remain_len > 0)
                            {
                                util_hex_dump("tcp data", (uint8_t*)ptcp_head + sizeof(tcp_header), remain_len);
                            }
                            else
                            {
                                printf("tcp data len: 0x%04X \n", remain_len);
                            }

                        }break;
                    case 0x11:
                        {
                            printf("====> UDP:\n");
                        }break;
                    case 0x58:
                        {
                            printf("IGRP\n");
                        }break;
                    case 0x59:
                        {
                            printf("OSPF\n");
                        }break;
                    default:
                        {
                            printf("Unknow protocol...\n");
                        }break;
                }
            }break;
        case 0x0806:
            {
                printf("ARP\n");
            }break;
        case 0x8035:
            {
                printf("RARP\n");
            }break;
        case 0x86DD:
            {
                printf("IPV6\n");
            }break;
        default:
            {
                printf("Unknow protocol type\n");
            }break;
    }
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
