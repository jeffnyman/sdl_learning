#ifndef LOGGER_SEEN
#define LOGGER_SEEN

#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <time.h>

// Data structures.

enum { LOG_TRACE, LOG_DEBUG, LOG_INFO, LOG_WARN, LOG_ERROR, LOG_FATAL };

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

#define log_trace(...) log_message(LOG_TRACE, __FILE__, __LINE__, __VA_ARGS__)
#define log_debug(...) log_message(LOG_DEBUG, __FILE__, __LINE__, __VA_ARGS__)
#define log_info(...)  log_message(LOG_INFO,  __FILE__, __LINE__, __VA_ARGS__)
#define log_warn(...)  log_message(LOG_WARN,  __FILE__, __LINE__, __VA_ARGS__)
#define log_error(...) log_message(LOG_ERROR, __FILE__, __LINE__, __VA_ARGS__)
#define log_fatal(...) log_message(LOG_FATAL, __FILE__, __LINE__, __VA_ARGS__)

// Function declarations.

void log_message(int, const char*, int, const char*, ...);
void log_set_level(int);
void log_set_quiet(bool);
int log_add_file_display(FILE*, int);
int log_add_display(log_display, void*, int);

#endif
