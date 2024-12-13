/*************************************************************************/ /**
 *****************************************************************************
 * @file   ls.c
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
 *                                ls
 *****************************************************************************/
/**
 * List all files.
 * 
 * @param the ouput string of all files.
 * 
 * @return 
 *****************************************************************************/
PUBLIC void ls(char *dst)
{
	MESSAGE msg;
	msg.type = LS;
	msg.BUF = dst;
	send_recv(BOTH, TASK_FS, &msg);

	return;
}
