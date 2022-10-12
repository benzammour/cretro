#include <stdarg.h>
#include <stdio.h>
#include <time.h>

#include "debug.h"

static debug_level_t min_dbg_lvl = fatal;

void debug_init(const config_t* conf) {
    min_dbg_lvl = conf->debug;

    // cap debug level, we want to see fatal errors at all times
    if (min_dbg_lvl > fatal)
        min_dbg_lvl = fatal;

    LOG_INFO("Log level initialized to %d.", min_dbg_lvl);
}

__attribute__((format(printf, 2, 3))) void _log_str(debug_level_t dbg_lvl, const char *msg, ...) {
    if (dbg_lvl < min_dbg_lvl) return;

    va_list args;
    time_t rawtime;
    struct tm * timeinfo;

    va_start(args, msg);
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    char* time_str = asctime(timeinfo);
    time_str[24] = '\0'; // terminate str early to eliminate newline (i hate this)

    const char* dbg_lvl_str = "unknown";
    switch (dbg_lvl) {
        case debug:
            dbg_lvl_str = "DEBUG";
            break;
        case info:
            dbg_lvl_str = "INFO";
            break;
        case warning:
            dbg_lvl_str = "WARNING";
            break;
        case error:
            dbg_lvl_str = "ERROR";
            break;
        case fatal:
            dbg_lvl_str = "FATAL";
            break;
        default:
            break;
    }
    printf("%10s – %s – ", dbg_lvl_str, time_str);
    vprintf(msg, args);
    printf("\n");

}
