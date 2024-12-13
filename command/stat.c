#include "type.h"
#include "stdio.h"
#include "string.h"

#define PATH_NAME_LEN (32)
#define ARG_NR_MAX (16)

int args;
char exe_buf[ARG_NR_MAX][PATH_NAME_LEN];

void disp_stat(struct stat *s)
{
	printf("st_dev -> %d\n", s->st_dev);
	printf("st_ino -> %d\n", s->st_ino);
	printf("st_mode -> %d\n", s->st_mode);
	printf("st_rdev -> %d\n", s->st_rdev);
	printf("st_size -> %d\n", s->st_size);
}

int main(int argc, char **argv)
{
	args = argc;
	for (int i = 0; i < args; i++) {
		memcpy(exe_buf[i], argv[i], PATH_NAME_LEN);
	}

	if (args != 2) {
		printf("stat usage: stat [filename]\n");
		return -1;
	}

	struct stat s;
	if (stat(exe_buf[1], &s) < 0) {
		printf("stat file %s error\n", exe_buf[1]);
		return -1;
	}

	disp_stat(&s);
	return 0;
}
