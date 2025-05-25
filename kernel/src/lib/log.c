#include <lib/log.h>
#include <lib/term.h>
#include <stddef.h>
#define NANOPRINTF_IMPLEMENTATION
#define NANOPRINTF_USE_FIELD_WIDTH_FORMAT_SPECIFIERS 1
#define NANOPRINTF_USE_PRECISION_FORMAT_SPECIFIERS 0
#define NANOPRINTF_USE_FLOAT_FORMAT_SPECIFIERS 0
#define NANOPRINTF_USE_LARGE_FORMAT_SPECIFIERS 1
#define NANOPRINTF_USE_BINARY_FORMAT_SPECIFIERS 1
#define NANOPRINTF_USE_WRITEBACK_FORMAT_SPECIFIERS 1
#include <lib/nanoprintf.h>

static const char *log_color[] = {
    [LOG_LEVEL_INFO] = "\033[0m",
    [LOG_LEVEL_WARN] = "\033[33m",
    [LOG_LEVEL_ERR] = "\033[31m"
};

/* TODO: exposing *printf functions to public may be a good idea */
int snprintf(char *__restrict buf, size_t bufsz,
        const char *__restrict fmt, ...) {
    va_list args;
    va_start(args, fmt);

    int ret = npf_vsnprintf(buf, bufsz, fmt, args);

    va_end(args);

    return ret;
}

int vsnprintf(char *__restrict buf, size_t bufsz,
        const char *__restrict fmt, va_list ap) {
    return npf_vsnprintf(buf, bufsz, fmt, ap);
}

#define LOG_BUFFER_SIZE 512

void log_write(log_level_t log_level, const char *fname,
        int lineno, const char *fnname,
        const char *__restrict format, ...)
{
    char buf[LOG_BUFFER_SIZE];
    size_t offset = 0;

    const char *color = log_color[log_level];
    const char *level_str = (log_level == LOG_LEVEL_INFO) ? "INFO" :
                            (log_level == LOG_LEVEL_WARN) ? "WARN" : "ERR";

    int written = snprintf(buf, LOG_BUFFER_SIZE,
            "%s[%s][%s:%d][%s] ",
            color, level_str, fname, lineno, fnname);

    if (written < 0 || (size_t)written >= LOG_BUFFER_SIZE) {
        /* truncated badly or failure */
        return;
    }

    offset += (size_t)written;

    va_list args;
    va_start(args, format);

    int user_msg_len = vsnprintf(buf + offset, LOG_BUFFER_SIZE - offset, format, args);

    va_end(args);

    if (user_msg_len < 0)
        return;

    offset += user_msg_len;

    const char *reset = "\033[0m\n";
    size_t reset_len = 5; /* strlen("\033[0m\n") */

    if (offset + reset_len >= LOG_BUFFER_SIZE) {
        reset_len = LOG_BUFFER_SIZE - offset - 1;
    }

    for (size_t i = 0; i < reset_len; i++) {
        buf[offset + i] = reset[i];
    }

    offset += reset_len;
    buf[offset] = '\0';

    term_write(buf, offset);
}
