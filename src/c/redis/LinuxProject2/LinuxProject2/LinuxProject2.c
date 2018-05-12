#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
	int fd;
	int ret;
	fd = open("./ABCD0001.txt", O_CREAT | O_RDWR, 0755);
	if(fd == -1)
	{
		perror("open error");
		exit(1);
	}
	ret = write(fd, "ABCD", 0x04);
	if(ret == -1)
	{
		perror("write error");
		exit(1);
	}
	return 0;
}