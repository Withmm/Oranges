/*************************************************************************/ /**
 *****************************************************************************
 * @file   ps.c
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
 *                                ps
 *****************************************************************************/
/**
 * List all running process
 * 
 * @param
 * 
 * @return Zero if successful, otherwise -1.
 *****************************************************************************/
PUBLIC void ps(char *dst)
{
	MESSAGE ps_msg;
	ps_msg.type = PS;
	ps_msg.BUF = dst;
	send_recv(BOTH, TASK_MM, &ps_msg);

	return;
}
