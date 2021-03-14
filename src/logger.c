#include "logger.h"

#define MAX_DISPLAYS 32

typedef struct {
    log_display display;
    void *udata;
    int level;
} logger_display;

static struct {
    void *udata;
    int level;
    bool quiet;
    logger_display displays[MAX_DISPLAYS];
} logger;

static const char *level_settings[] = {
    "TRACE", "DEBUG", "INFO", "WARN", "ERROR", "FATAL"
};

#ifdef LOG_USE_COLOR
static const char *level_colors[] = {
    "\x1b[94m", "\x1b[36m", "\x1b[32m", "\x1b[33m", "\x1b[31m", "\x1b[35m"
};
#endif

void log_set_level(int level) {
    logger.level = level;
}

void log_set_quiet(bool enable) {
    logger.quiet = enable;
}

static void file_display(log_event *ev) {
    char buffer[64];

    buffer[strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", ev -> time)] = '\0';

    fprintf(
        ev -> udata,
        "%s %-5s %s:%d: ",
        buffer,
        level_settings[ev -> level],
        ev -> file,
        ev -> line
    );

    vfprintf(ev -> udata, ev -> message, ev -> ap);
    fprintf(ev -> udata, "\n");
    fflush(ev -> udata);
}

int log_add_display(log_display display, void *udata, int level) {
    for (int i = 0; i < MAX_DISPLAYS; i++) {
        if (!logger.displays[i].display) {
            logger.displays[i] = (logger_display) { display, udata, level };
            return 0;
        }
    }

    return -1;
}

int log_add_file_display(FILE *fp, int level) {
    return log_add_display(file_display, fp, level);
}

#ifdef LOG_USE
static void init_event(log_event *ev, void *udata) {
    if (!ev -> time) {
        time_t t = time(NULL);
        ev -> time = localtime(&t);
    }

    ev -> udata = udata;
}

static void stdout_display(log_event *ev) {
    char buffer[16];

    buffer[strftime(buffer, sizeof(buffer), "%H:%M:%S", ev -> time)] = '\0';

#ifdef LOG_USE_COLOR
    fprintf(
        ev -> udata,
        "%s %s%-5s\x1b[0m \x1b[90m%s:%d:\x1b[0m ",
        buffer,
        level_colors[ev -> level],
        level_settings[ev -> level],
        ev -> file,
        ev -> line
    );
#else
    fprintf(
        ev -> udata,
        "%s %-5s %s:%d: ",
        buffer,
        level_settings[ev -> level],
        ev -> file,
        ev -> line
    );
#endif

    vfprintf(ev -> udata, ev -> message, ev -> ap);
    fprintf(ev -> udata, "\n");
    fflush(ev -> udata);
}
#endif

void log_message(int level, const char *file, int line, const char *message, ...) {
    // Doing the following to avoid -Wunused-parameter warnings.
    (void)level;
    (void)file;
    (void)line;
    (void)message;

#ifdef LOG_USE
log_event ev = {
    .message = message,
    .file    = file,
    .line    = line,
    .level   = level,
};

if (!logger.quiet && level >= logger.level) {
    init_event(&ev, stderr);
    va_start(ev.ap, message);
    stdout_display(&ev);
    va_end(ev.ap);
}

for (int i = 0; i < MAX_DISPLAYS && logger.displays[i].display; i++) {
    logger_display *ld = &logger.displays[i];

    if (level >= ld -> level) {
        init_event(&ev, ld -> udata);
        va_start(ev.ap, message);
        ld -> display(&ev);
        va_end(ev.ap);
    }
}
#endif
}
