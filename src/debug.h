#ifndef _CH8_SRC_DEBUG_H_
#define _CH8_SRC_DEBUG_H_

#include "cli.h"

typedef enum {
    DEBUG,
    INFO,
    WARNING,
    ERROR,
    FATAL
} debug_level_t;

void _log_str(debug_level_t dbg_lvl, const char* msg, ...);

void debug_init(const config_t* conf);

#define LOG_DEBUG(msg, ...)     _log_str(DEBUG, msg, ##__VA_ARGS__)
#define LOG_INFO(msg, ...)      _log_str(INFO, msg, ##__VA_ARGS__)
#define LOG_WARNING(msg, ...)   _log_str(WARNING, msg, ##__VA_ARGS__)
#define LOG_ERROR(msg, ...)     _log_str(ERROR, msg, ##__VA_ARGS__)
#define LOG_FATAL(msg, ...)     _log_str(FATAL, msg, ##__VA_ARGS__)

#endif /* _CH8_SRC_DEBUG_H_ */
