#include <stdio.h>
#include <stdarg.h>
#include <sys/time.h>
#include "time.h"
#include "log.h"

static int s_log_level = LOG_LEVEL_INFO;

void log_set_level(int level)
{
    s_log_level = level;
}

int log_get_level(void)
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

void test_log()
{
    int log_level;

    log_level = log_get_level();
    for(log_level = LOG_LEVEL_NONE; log_level <= LOG_LEVEL_VERB; log_level++)
    {
        log_set_level(log_level);
        printf("log level = %d\n", log_level);
        log_error("error message\n");
        log_warning("warning message\n");
        log_info("info message\n");
        log_debug("debug message\n");
        log_develop("develop message\n");
        log_verbose("verbose message\n");
        printf("================================================================================\n");
    }
}

