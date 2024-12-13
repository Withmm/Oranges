/*************************************************************************/ /**
 *****************************************************************************
 * @file   log.c
 * @brief  
 * @author Zile Xiong 
 * @date   2024 
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
#include "hd.h"
#include "fs.h"
#include "log.h"

void log(u8 level, const char *fmt, ...)
{
	int n;
	char buf[STR_DEFAULT_LEN];

	va_list arg = (va_list)((char *)(&fmt) +
				4); /* 4 是参数 fmt 所占堆栈中的大小 */
	n = vsprintf(buf, fmt, arg);

	if (w_logpos + sizeof(LOG_ENTRY) + n >= LOG_END) {
		w_logpos = (u32)logbuf;
	}

	while (w_logpos + sizeof(LOG_ENTRY) + n >= r_logpos - 1) {
		// wait;
	}

	LOG_ENTRY *tmp = (LOG_ENTRY *)w_logpos;
	tmp->level = level;
	tmp->len = n;
	memcpy(tmp->msg, buf, n);
	w_logpos += sizeof(*tmp) + n;
}

