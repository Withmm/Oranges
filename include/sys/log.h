/*
 * log.h
 *
 * Description: Header file for the in-memory logging system. 
 *              Defines structures, macros, and function prototypes 
 *              for logging to a specific memory region.
 *
 * Author: Zile Xiong
 * Major: Information Security
 * Email: xiongzile99@qq.com
 *
 * Date Created: December 13, 2024
 * Last Updated: December 13, 2024
 *
 * License: MIT (or specify any applicable license)
 *
 * Usage: Include this file in your project to enable memory-based logging.
 */
#ifndef _ORANGES_LOG_H
#define _ORANGES_LOG_H

#include "type.h"
void log(u8 level, const char *fmt, ...);

typedef struct log_entry {
	u8 level;
	u32 len;
	char msg[];
}LOG_ENTRY;


#define LOG(level, fmt, ...) log(level, fmt, ##__VA_ARGS__)


#define ERROR(fmt, ...) LOG(LOG_ERROR, fmt, ##__VA_ARGS__)
#define WARN(fmt, ...)  LOG(LOG_WARN, fmt, ##__VA_ARGS__)
#define INFO(fmt, ...)	LOG(LOG_INFO, fmt, ##__VA_ARGS__)
#define DEBUG(fmt, ...) LOG(LOG_DEBUG, fmt, ##__VA_ARGS__)


#endif
