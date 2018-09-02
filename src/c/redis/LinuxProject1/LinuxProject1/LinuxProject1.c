#include <stdio.h>
#include "unistd.h"
#include "netinet/in.h"

int main(int argc, char *argv[])
{
	int i;
	for (i = 0; i < 10; ++i)
	{
		printf("%d\n", i);
	}
	char sz[] = "Hello, World!\n";	/* Hover mouse over "sz" while debugging to see its contents */
	printf("%s", sz);	
	fflush(stdout); /* <============== Put a breakpoint here */
	return 0;
}