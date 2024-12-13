/*************************************************************************/ /**
 *****************************************************************************
 * @file   mm/main.c
 * @brief  Orange'S Memory Management.
 * @author Forrest Y. Yu
 * @date   Tue May  6 00:33:39 2008
 *****************************************************************************
 *****************************************************************************/

#include "type.h"
#include "config.h"
#include "stdio.h"
#include "const.h"
#include "protect.h"
#include "string.h"
#include "fs.h"
#include "proc.h"
#include "tty.h"
#include "console.h"
#include "global.h"
#include "keyboard.h"
#include "proto.h"

PUBLIC void do_fork_test();
PRIVATE void do_ps();
PRIVATE int do_kill();
PRIVATE void init_mm();

/*****************************************************************************
 *                                task_mm
 *****************************************************************************/
/**
 * <Ring 1> The main loop of TASK MM.
 * 
 *****************************************************************************/
PUBLIC void task_mm()
{
	init_mm();

	while (1) {
		send_recv(RECEIVE, ANY, &mm_msg);
		int src = mm_msg.source;
		int reply = 1;

		int msgtype = mm_msg.type;

		switch (msgtype) {
		case FORK:
			mm_msg.RETVAL = do_fork();
			break;
		case EXIT:
			do_exit(mm_msg.STATUS);
			reply = 0;
			break;
		case EXEC:
			mm_msg.RETVAL = do_exec();
			break;
		case WAIT:
			do_wait();
			reply = 0;
			break;
		case PS:
			do_ps();
			break;
		case KILL:
			mm_msg.RETVAL = do_kill();
			break;
		default:
			dump_msg("MM::unknown msg", &mm_msg);
			assert(0);
			break;
		}

		if (reply) {
			mm_msg.type = SYSCALL_RET;
			send_recv(SEND, src, &mm_msg);
		}
	}
}

/*****************************************************************************
 *                                init_mm
 *****************************************************************************/
/**
 * Do some initialization work.
 * 
 *****************************************************************************/
PRIVATE void init_mm()
{
	struct boot_params bp;
	get_boot_params(&bp);

	memory_size = bp.mem_size;

	/* print memory size */
	printl("{MM} memsize:%dMB\n", memory_size / (1024 * 1024));
}

/*****************************************************************************
 *                                alloc_mem
 *****************************************************************************/
/**
 * Allocate a memory block for a proc.
 * 
 * @param pid  Which proc the memory is for.
 * @param memsize  How many bytes is needed.
 * 
 * @return  The base of the memory just allocated.
 *****************************************************************************/
PUBLIC int alloc_mem(int pid, int memsize)
{
	assert(pid >= (NR_TASKS + NR_NATIVE_PROCS));
	if (memsize > PROC_IMAGE_SIZE_DEFAULT) {
		panic("unsupported memory request: %d. "
		      "(should be less than %d)",
		      memsize, PROC_IMAGE_SIZE_DEFAULT);
	}

	int base = PROCS_BASE + (pid - (NR_TASKS + NR_NATIVE_PROCS)) *
					PROC_IMAGE_SIZE_DEFAULT;

	if (base + memsize >= memory_size)
		panic("memory allocation failed. pid:%d", pid);

	return base;
}

/*****************************************************************************
 *                                free_mem
 *****************************************************************************/
/**
 * Free a memory block. Because a memory block is corresponding with a PID, so
 * we don't need to really `free' anything. In another word, a memory block is
 * dedicated to one and only one PID, no matter what proc actually uses this
 * PID.
 * 
 * @param pid  Whose memory is to be freed.
 * 
 * @return  Zero if success.
 *****************************************************************************/
PUBLIC int free_mem(int pid)
{
	return 0;
}

PRIVATE void do_ps()
{
	int src = mm_msg.source;
	char *p_buf = mm_msg.BUF;

	struct proc *p = proc_table;
	int count = 0;
	int l = 0;

#define WRITETOBUF(X)                                    \
	do {                                             \
		l = strlen(X);                           \
		phys_copy((void *)va2la(src, p_buf),     \
			  (void *)va2la(TASK_MM, X), l); \
		p_buf += l;                              \
	} while (0)

	while (p->p_flags != FREE_SLOT) {
		char pid_buf[4] = { 0 };
		sprintf(pid_buf, "%d", count++);
		WRITETOBUF(pid_buf);
		WRITETOBUF("        ");
		WRITETOBUF(p->name);
		WRITETOBUF("        ");
		switch (p->p_flags) {
#define XX(A, B)               \
	case A:                \
		WRITETOBUF(B); \
		break

			XX(0x00, "RUNNING");
			XX(0x02, "SENDING");
			XX(0x04, "RECEIVING");
			XX(0x08, "WAITING");
			XX(0x10, "HANGING");
			XX(0x20, "FREE_SLOT");
			XX(0x40, "BLOCKED");

		default:
			WRITETOBUF("RUNNABLE");
			break;
		}
		WRITETOBUF("\n");

		++p;
	}
}

PRIVATE int do_kill()
{
	int pid = mm_msg.PROC_NR;
	int state = mm_msg.FLAGS;
	if (pid < NR_TASKS + 1 || pid >= NR_TASKS + NR_PROCS) {
		return -1; //means connot send kill msg to this procs.
	}
	proc_table[pid].p_flags = state;
	return 0;
}
