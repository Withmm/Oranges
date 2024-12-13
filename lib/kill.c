/*************************************************************************/ /**
 *****************************************************************************
 * @file   kill.c
 * @brief  
 * @author Zile Xiong
 * @date   2024 
 *****************************************************************************
 *****************************************************************************/

#include "type.h"
#include "stdio.h"
#include "const.h"
#include "protect.h"
#include "string.h"
#include "fs.h"
#include "proc.h"
#include "tty.h"
#include "console.h"
#include "global.h"
#include "proto.h"

/*****************************************************************************
 *                                kill
 *****************************************************************************/
/**
 * signal
 * 
 * @param 
 * 
 * @return Zero if successful, otherwise -1.
 *****************************************************************************/
PUBLIC int kill(int pid, int state)
{
	MESSAGE msg;
	msg.type = KILL;
	msg.PROC_NR = pid;
	msg.FLAGS = state;

	send_recv(BOTH, TASK_MM, &msg);

	return msg.RETVAL;
}
