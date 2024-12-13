#include "stdio.h"

#define PROCESS_NAME_LEN (1024)

int main(int argc, char **argv)
{
	char buf[PROCESS_NAME_LEN] = { 0 };
	ps(buf);
	printf("PID        PNAME\n");
	printf(buf);
	return 0;
}
