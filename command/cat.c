#include "type.h"
#include "stdio.h"
#include "string.h"

#define PATH_NAME_LEN (32)
#define ARG_NR_MAX (16)

int args;
char exe_buf[ARG_NR_MAX][PATH_NAME_LEN];
int main(int argc, char **argv)
{
	args = argc;
	for (int i = 0; i < args; i++) {
		memcpy(exe_buf[i], argv[i], PATH_NAME_LEN);
	}
	if (args != 2) {
		printf("cat usage: cat [filename]\n");
		return -1;
	}

	int fd = open(exe_buf[1], O_RDWR);
	if (fd < 0) {
		printf("unable to open file: %s\n", exe_buf[1]);
		return -1;
	}

	char buf[1024] = { 0 };
	int n = read(fd, buf, 1024);
	if (n < 0) {
		printf("failed to read file error: %s\n", exe_buf[1]);
		return -1;
	}
	printf(buf);
	printf("\n");
	close(fd);
	return 0;
}
