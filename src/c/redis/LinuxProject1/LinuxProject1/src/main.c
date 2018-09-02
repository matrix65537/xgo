#include <stdio.h>
#include <sys/epoll.h>

int main(int argc, char** argv)
{
	int fd;
	int i;
	for (i = 0; i < 1000; ++i)
	{
		fd = epoll_create(1024);
		printf("HelloWorld! %d\n", fd);
	}
	return 0;
}

