#include "type.h"
#include "stdio.h"
#include "string.h"

#define LS_MAX 1024

int main(int argc, char **argv)
{
	if (argc != 1) {
		printf("ls usage: ls\n");
		return -1;
	}

	char buf[LS_MAX] = { 0 };
	ls(buf);

	printf("%s\n", buf);
	return 0;
}
