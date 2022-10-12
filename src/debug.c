#include <stdarg.h>
#include <stdio.h>
#include <time.h>

#include "debug.h"

static debug_level_t min_dbg_lvl = FATAL;

void debug_init(const config_t* conf) {
    min_dbg_lvl = conf->debug;

    // minimum debug level cannot be higher than fatal
    if (min_dbg_lvl > FATAL)
        min_dbg_lvl = FATAL;

    // minimum debug level cannot be lower than debug
    if (min_dbg_lvl < DEBUG)
        min_dbg_lvl = DEBUG;

    LOG_INFO("Log level initialized to %d.", min_dbg_lvl);
}

__attribute__((format(printf, 2, 3))) void _log_str(debug_level_t dbg_lvl, const char *msg, ...) {
    if (dbg_lvl < min_dbg_lvl) return;

    va_list args;
    time_t rawtime;
    struct tm * timeinfo;
    FILE* stream;

    va_start(args, msg);
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    char* time_str = asctime(timeinfo);
    time_str[24] = '\0'; // terminate str early to eliminate newline (i hate this)

    const char* dbg_lvl_str = "unknown";
    switch (dbg_lvl) {
        case DEBUG:
            dbg_lvl_str = "DEBUG";
            stream = stdout;
            break;
        case INFO:
            dbg_lvl_str = "INFO";
            stream = stdout;
            break;
        case WARNING:
            dbg_lvl_str = "WARNING";
            stream = stdout;
            break;
        case ERROR:
            dbg_lvl_str = "ERROR";
            stream = stderr;
            break;
        case FATAL:
            dbg_lvl_str = "FATAL";
            stream = stderr;
            break;
        default:
            stream = stderr;
            break;
    }
    fprintf(stream, "%10s – %s – ", dbg_lvl_str, time_str);
    vfprintf(stream, msg, args);
    fprintf(stream, "\n");

}
