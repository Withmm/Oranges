#include "type.h"
#include "stdio.h"
#include "string.h"

#define ELF_MAGIC_LEN (4)
#define PATH_NAME_LEN (32)
#define ARG_NR_MAX (16)
#define EDIT_BUF_SIZE (8192)
#define READ_ONCE_LEN (256)
int args;
char exe_buf[ARG_NR_MAX][PATH_NAME_LEN];
char edit_buf[EDIT_BUF_SIZE];
int edit_pos;

void write_buf(int pos)
{
	if (pos + READ_ONCE_LEN >= EDIT_BUF_SIZE) {
		printf("edit buffer overflow!\n");
		return;
	}

	int n = read(0, &edit_buf[pos], READ_ONCE_LEN);
	if (n < 0) {
		printf("do_append(): failed read from stdin.\n");
		return;
	}
	edit_pos = pos + n;
}

void do_append()
{
	write_buf(edit_pos);
}

void do_trunc()
{
	memset(edit_buf, 0, EDIT_BUF_SIZE);
	write_buf(0);
}

void writeback(char *pathname)
{
	int fd = open(pathname, O_RDWR);
	if (fd < 0) {
		printf("writeback(): Failed to open file %s.\n", pathname);
		return;
	}
	struct stat s;
	if (stat(pathname, &s) < 0) {
		printf("writeback::stat(): Failed to open file %s.\n",
		       pathname);
	}

	if (s.st_size > EDIT_BUF_SIZE) {
		printf("writeback: edit buffer overflow!\n");
		close(fd);
		return;
	}
	write(fd, edit_buf, s.st_size);
	close(fd);
	return;
}
void edit(char *pathname)
{
	int fd = open(pathname, O_RDWR);
	if (fd < 0) {
		printf("Failed to open file %s\n", pathname);
		return;
	}

	struct stat s;
	if (stat(pathname, &s) < 0) {
		printf("Failed to stat file %s\n", pathname);
	}

	int file_size = s.st_size;
	int n = read(fd, edit_buf, file_size);
	edit_pos = n;
	if (edit_pos >= EDIT_BUF_SIZE) {
		printf("cannot open too big file: %s\n", pathname);
		return;
	}
	edit_pos = 0;
	while (edit_buf[edit_pos])
		++edit_pos;
	printf("%s\n", edit_buf);
	printf("edit_pos = %d\n", edit_pos);
	close(fd);
	while (1) {
		printf("Please choose your edit mode\n");
		printf("1 -> append\n");
		printf("2 -> trunc\n");
		printf("3 -> quit\n");
		printf("4 -> save and quit\n");
		char cbuf[8] = { 0 };
		int r = read(0, cbuf, 4);
		cbuf[r] = 0;

		switch (cbuf[0]) {
		case '1':
			do_append();
			break;
		case '2':
			do_trunc();
			break;
		case '3':
			return;
		case '4':
			writeback(pathname);
			return;
		default:
			printf("Please enter one of \"1 2 3 4\"\n");
			return;
		}
	}
}

void exe(int args, char **argv)
{
	execv(argv[0], argv);
}
int main(int argc, char **argv)
{
	args = argc;
	for (int i = 0; i < args; i++) {
		memcpy(exe_buf[i], argv[i], PATH_NAME_LEN);
	}

	if (args < 2) {
		printf("edit usage: edit [filename] or edit [exe ...]\n");
		return -1;
	}

	/*
	for (int i = 0; i < args; i++) {
		printf("exe_buf[%d] = %s ", i, exe_buf[i]);
	}
	*/

	int fd = open(exe_buf[1], O_RDWR);
	if (fd < 0) {
		printf("Faild to open file %s\n", exe_buf[1]);
		return -1;
	}

	u8 elfbuf[ELF_MAGIC_LEN] = { 0 };
	int n = read(fd, elfbuf, ELF_MAGIC_LEN);
	if (n != ELF_MAGIC_LEN) {
		printf("failed to read from file: %s\n", exe_buf[1]);
		return -1;
	}
	if (!(elfbuf[0] == 0x7f && elfbuf[1] == 'E' && elfbuf[2] == 'L' &&
	      elfbuf[3] == 'F')) {
		close(fd);
		edit(exe_buf[1]);
	} else {
		close(fd);
		char *tmp[ARG_NR_MAX] = { 0 };
		for (int i = 0; i < args - 1; i++) {
			tmp[i] = exe_buf[i + 1];
		}
		tmp[args - 1] = 0;
		exe(args - 1, tmp);
	}
}
