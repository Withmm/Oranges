#include "type.h"
#include "stdio.h"
#include "string.h"

#define PATH_NAME_LEN (32)
#define ARG_NR_MAX (16)

int args;
char exe_buf[ARG_NR_MAX][PATH_NAME_LEN];
int main(int argc, char *argv[])
{
	args = argc;
	for (int i = 0; i < args; i++) {
		memcpy(exe_buf[i], argv[i], PATH_NAME_LEN);
	}

	int i;
	for (i = 1; i < args; i++)
		printf("%s%s", i == 1 ? "" : " ", exe_buf[i]);
	printf("\n");

	return 0;
}
