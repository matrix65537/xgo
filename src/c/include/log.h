#ifndef __LOG_H__
#define __LOG_H__

#define __WIN32__

enum
{
    LOG_LEVEL_NONE  = 0,
    LOG_LEVEL_ERRO  = 1,
    LOG_LEVEL_WARN  = 2,
    LOG_LEVEL_INFO  = 3,
    LOG_LEVEL_DBUG  = 4,
    LOG_LEVEL_DEVP  = 5,
    LOG_LEVEL_VERB  = 6,
};

void log_set_level(int level);

int log_get_level();

void log_output(int level, const char* fmt, ...);

#ifdef __WIN32__
	#define log_error(fmt, ...)     log_output(LOG_LEVEL_ERRO, fmt, __VA_ARGS__);
	#define log_warning(fmt, ...)   log_output(LOG_LEVEL_WARN, fmt, __VA_ARGS__);
	#define log_info(fmt, ...)      log_output(LOG_LEVEL_INFO, fmt, __VA_ARGS__);
	#define log_debug(fmt, ...)     log_output(LOG_LEVEL_DBUG, fmt, __VA_ARGS__);
	#define log_develop(fmt, ...)   log_output(LOG_LEVEL_DEVP, fmt, __VA_ARGS__);
	#define log_verbose(fmt, ...)   log_output(LOG_LEVEL_VERB, fmt, __VA_ARGS__);
#else
	#define log_error(fmt, arg ...)     log_output(LOG_LEVEL_ERRO, fmt, ##arg);
	#define log_warning(fmt, arg ...)   log_output(LOG_LEVEL_WARN, fmt, ##arg);
	#define log_info(fmt, arg ...)      log_output(LOG_LEVEL_INFO, fmt, ##arg);
	#define log_debug(fmt, arg ...)     log_output(LOG_LEVEL_DBUG, fmt, ##arg);
	#define log_develop(fmt, arg ...)   log_output(LOG_LEVEL_DEVP, fmt, ##arg);
	#define log_verbose(fmt, arg ...)   log_output(LOG_LEVEL_VERB, fmt, ##arg);
#endif

#endif
