#ifndef _CH8_SRC_DEBUG_H_
#define _CH8_SRC_DEBUG_H_

#include "cli.h"

typedef enum {
    DEBUG,
    INFO,
    WARNING,
    ERROR,
    FATAL
} log_level_t;

void log_str(log_level_t dbg_lvl, const char* msg, ...);

void log_init(const config_t* conf);

#define LOG_DEBUG(msg, ...)     log_str(DEBUG, msg, ##__VA_ARGS__)
#define LOG_INFO(msg, ...)      log_str(INFO, msg, ##__VA_ARGS__)
#define LOG_WARNING(msg, ...)   log_str(WARNING, msg, ##__VA_ARGS__)
#define LOG_ERROR(msg, ...)     log_str(ERROR, msg, ##__VA_ARGS__)
#define LOG_FATAL(msg, ...)     log_str(FATAL, msg, ##__VA_ARGS__)

#endif /* _CH8_SRC_DEBUG_H_ */
