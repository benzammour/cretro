/**
 *
 * @file	debug.h
 * @brief 	Header file for debugging window
 * @author 	Moritz R. Huellmann
 * @date 	10.10.22
 *
 */

#ifndef _CH8_SRC_DEBUG_H_
#define _CH8_SRC_DEBUG_H_

#include "cli.h"

typedef enum {
    debug,
    info,
    warning,
    error,
    fatal
} debug_level_t;

void _log_str(debug_level_t dbg_lvl, const char* msg, ...);

void debug_init(const config_t* conf);

#define LOG_DEBUG(msg, ...)     _log_str(debug, msg, ##__VA_ARGS__)
#define LOG_INFO(msg, ...)      _log_str(info, msg, ##__VA_ARGS__)
#define LOG_WARNING(msg, ...)   _log_str(warning, msg, ##__VA_ARGS__)
#define LOG_ERROR(msg, ...)     _log_str(error, msg, ##__VA_ARGS__)
#define LOG_FATAL(msg, ...)     _log_str(fatal, msg, ##__VA_ARGS__)

#endif /* _CH8_SRC_DEBUG_H_ */
