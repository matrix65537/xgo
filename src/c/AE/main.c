#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "ae.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include "ring.h"
#include "log.h"
#include "util.h"

enum{
	MAX_DATA_SIZE	=	0x04,
};

typedef struct DataNode{
	uint32_t read_offset;
	uint32_t write_offset;
	uint8_t data[MAX_DATA_SIZE];
}DataNode;

#define LISTEN_PORT 9090

#define TIMER1_MS	3000
#define TIMER2_MS	5000

int Timer1(struct aeEventLoop* eventLoop, long long id, void* clientData)
{
	log_info("Timer1: %d %d\n", eventLoop->timeEventHead->when_sec, eventLoop->timeEventHead->when_ms);
	return TIMER1_MS;
}

int Timer2(struct aeEventLoop* eventLoop, long long id, void* clientData)
{
	log_info("        Timer2\n");
	return TIMER2_MS;
}

extern void ListenProc(struct aeEventLoop* eventLoop, int fd, void* clientData, int mask);
extern void RWProc(struct aeEventLoop* eventLoop, int fd, void* clientData, int mask);
extern void WriteProc(struct aeEventLoop* eventLoop, int fd, void* clientData, int mask);

void ListenProc(struct aeEventLoop* eventLoop, int fd, void* clientData, int mask)
{
	int ret;
	struct sockaddr_in client_addr;
	int sock_addr_len = sizeof(client_addr);

	ret = accept(fd, (struct sockaddr*)&client_addr, &sock_addr_len); 
	if(ret == INVALID_SOCKET)
	{
		log_error("accept error.\n");
	}
	else
	{
		Ring* ring = create_ring(RING_TEMP_BUF_SIZE);
        char* paddr = inet_ntoa(client_addr.sin_addr);
		log_info("Accepted client: %s:%d fd = %d\n", paddr, ntohs(client_addr.sin_port), ret); 
		aeCreateFileEvent(eventLoop, ret, AE_READABLE, RWProc, ring);
	}
}

void RWProc(struct aeEventLoop* eventLoop, int fd, void* clientData, int mask)
{
	int ret;
	int size;
	Ring* ring = (Ring*)clientData;

	aeDeleteFileEvent(eventLoop, fd, AE_READABLE | AE_WRITABLE);
	if(mask & AE_READABLE)
	{
		log_debug("ReadProc\n");
		size = get_remain_size(ring);
		ret = recv(fd, ring->temp_buf, size, 0);

		if(ret <= 0)
		{
			log_info("client exit, close socket %d.\n", fd);
			close(fd);
			delete_ring(ring);
		}
		else
		{
			util_hex_dump("recv", ring->temp_buf, ret);
			ring_write_data(ring, ring->temp_buf, ret);

			//接收到数据了，可对外写
			aeCreateFileEvent(eventLoop, fd, AE_WRITABLE, RWProc, clientData);
			//如果没满，也可读
			if(!ring_is_full(ring))
			{
				aeCreateFileEvent(eventLoop, fd, AE_READABLE, RWProc, clientData);
			}
		}
	}
	else if(mask & AE_WRITABLE)
	{
		log_debug("WriteProc\n");
		size = get_data_size(ring);
		ring_read_data(ring, ring->temp_buf, size);
		ret = send(fd, ring->temp_buf, size, 0);

		if(ret <= 0)
		{
			log_error("client exit, close socket.\n");
			close(fd);
			delete_ring(ring);
		}
		else
		{
			util_hex_dump("send", ring->temp_buf, ret);
			//发送之后，有剩余空间，可读
			aeCreateFileEvent(eventLoop, fd, AE_READABLE, RWProc, clientData);
			//如果不是空，则可写（可对外发送）
			if(!ring_is_empty(ring))
			{
				aeCreateFileEvent(eventLoop, fd, AE_WRITABLE, RWProc, clientData);
			}
		}
	}
	else
	{
	}
}

extern void test_log();
int main()
{
	int server_listen_fd;
	struct sockaddr_in server_addr;
	int server_addr_len;
	int on = 1;
	int r;
	aeEventLoop* eventLoop = aeCreateEventLoop();

	log_set_level(LOG_LEVEL_DEVP);

	server_listen_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_listen_fd == INVALID_SOCKET)
	{
        perror("socket error");
		exit(1);
	}

	if (setsockopt(server_listen_fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) == -1)
	{
        perror("setsockopt error");
		exit(1);
	}

	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(LISTEN_PORT);

	server_addr_len = sizeof(server_addr);
	r = bind(server_listen_fd, (struct sockaddr*)&server_addr, server_addr_len);
	if (r == -1)
	{
        perror("bind error");
		exit(1);
	}
	r = listen(server_listen_fd, 0x100);
	if (r == -1)
	{
        perror("listen error");
		exit(1);
	}

	aeCreateTimeEvent(eventLoop, TIMER1_MS, Timer1, NULL, NULL);
	aeCreateTimeEvent(eventLoop, TIMER2_MS, Timer2, NULL, NULL);

	aeCreateFileEvent(eventLoop, server_listen_fd, AE_READABLE, ListenProc, NULL);

	aeMain(eventLoop);

	return 0;
}
