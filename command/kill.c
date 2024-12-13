#include "type.h"
#include "stdio.h"
#include "string.h"

#define PATH_NAME_LEN (32)
#define ARG_NR_MAX (16)

int args;
char exe_buf[ARG_NR_MAX][PATH_NAME_LEN];
int atoi(const char *str)
{
	int num = 0;
	while (*str >= '0' && *str <= '9') {
		num = num * 10 + (*str - '0');
		str++;
	}

	return num;
}
int main(int argc, char **argv)
{
	args = argc;
	for (int i = 0; i < args; i++) {
		memcpy(exe_buf[i], argv[i], PATH_NAME_LEN);
	}

	if (args != 2 && args != 3) {
		printf("kill usage: kill help\n");
		return -1;
	}

	if (args == 2) {
		if (memcmp(exe_buf[1], "help", 5) == 0) {
			printf("kill usage:\n");
			printf("kill -0 [pid] for blocking a process.\n");
			printf("kill -1 [pid] for unblocking a process.\n");
			return -1;
		} else {
			printf("kill usage: kill help\n");
			return -1;
		}
	}
	if (args == 3) {
		char state_buf[4] = { 0 };
		memcpy(state_buf, exe_buf[1], 2);
		char pid_buf[4] = { 0 };
		memcpy(pid_buf, exe_buf[2], 4);
		if (memcmp(state_buf, "-0", 3) == 0) {
			//send unblock
			int state = 0;
			int pid = atoi(pid_buf);
			if (kill(pid, state) == -1) {
				printf("kill pid %d failed\n", pid);
				return -1;
			}

		} else if (memcmp(state_buf, "-1", 3) == 0) {
			//send block
			int state = 0x40;
			int pid = atoi(pid_buf);
			if (kill(pid, state) == -1) {
				printf("kill pid %d failed\n", pid);
				return -1;
			}
		} else {
			printf("kill usage: kill help\n");
			return -1;
		}
	}
	return 0;
}
