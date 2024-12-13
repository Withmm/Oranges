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

	if (args < 2) {
		printf("rm usage: rm [filename1] [filename2] ...\n");
		return -1;
	}
	for (int i = 1; i < args; i++) {
		if (unlink(exe_buf[i]) < 0)
			printf("failed to rm file %s\n", exe_buf[i]);
	}
	return 0;
}
