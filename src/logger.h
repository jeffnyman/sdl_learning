#ifndef LOGGER_SEEN
#define LOGGER_SEEN

#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <time.h>

#ifndef DEFAULT_LOG_LEVEL
#define DEFAULT_LOG_LEVEL 4
#endif

// Data structures.

enum {
    LOG_DEBUG = 0,
    LOG_TRACE = 1,
    LOG_INFO  = 2,
    LOG_WARN  = 3,
    LOG_ERROR = 4,
    LOG_FATAL = 5,
};

typedef struct {
    va_list ap;
    const char *message;
    const char *file;
    struct tm *time;
    void *udata;
    int line;
    int level;
} log_event;

typedef void (*log_display)(log_event *ev);

// Macro definitions.

#define log_debug(...) do { if (LOG_DEBUG == DEFAULT_LOG_LEVEL) log_message(LOG_DEBUG, __FILE__, __LINE__, __VA_ARGS__); } while(0)
#define log_trace(...) do { if (LOG_TRACE == DEFAULT_LOG_LEVEL) log_message(LOG_TRACE, __FILE__, __LINE__, __VA_ARGS__); } while(0)
#define log_info(...)  do { if (LOG_INFO == DEFAULT_LOG_LEVEL) log_message(LOG_INFO,  __FILE__, __LINE__, __VA_ARGS__); } while(0)
#define log_warn(...)  do { if (LOG_WARN == DEFAULT_LOG_LEVEL) log_message(LOG_WARN,  __FILE__, __LINE__, __VA_ARGS__); } while(0)
#define log_error(...) do { if (LOG_ERROR == DEFAULT_LOG_LEVEL) log_message(LOG_ERROR, __FILE__, __LINE__, __VA_ARGS__); } while(0)
#define log_fatal(...) do { if (LOG_FATAL == DEFAULT_LOG_LEVEL) log_message(LOG_FATAL, __FILE__, __LINE__, __VA_ARGS__); } while(0)

// Function declarations.

void log_message(int, const char*, int, const char*, ...);
void log_set_level(int);
void log_set_quiet(bool);
int log_add_file_display(FILE*, int);
int log_add_display(log_display, void*, int);

#endif
