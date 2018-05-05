#include <stdio.h>
#include <stdarg.h>
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

void log_output(int level, const char* fmt, ...)
{
    va_list args;

    if(level > s_log_level)
    {
        return;
    }
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
}

void test_log()
{
	int log_level;
	log_set_level(LOG_LEVEL_INFO);
	log_level = log_get_level();

	log_output(LOG_LEVEL_DBUG, "hello: %d %s %d", 1, "ABCD", 2);
	log_output(LOG_LEVEL_DBUG, "hello: %d %s %d", 1, "ABCD", 2);
}