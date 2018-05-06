#include <stdio.h>
#include <stdarg.h>
#include <sys/time.h>
#include "time.h"
#include "log.h"

int s_log_level = LOG_LEVEL_DBUG;

void log_set_level(int level)
{
    s_log_level = level;
}

int log_get_level()
{
    return s_log_level;
}

void log_output(const char* tag, int level, const char* fmt, ...)
{
    time_t t;
    char buf[0x40];
    va_list args;

    if(level > s_log_level)
    {
        return;
    }
    t = time(NULL);
    strftime(buf, sizeof(buf), "%F %T", localtime(&t));

    printf("[%s][%s] ", buf, tag);
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
}

