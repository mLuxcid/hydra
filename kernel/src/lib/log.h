#ifndef _HYDRA_LIB_LOG_H
#define _HYDRA_LIB_LOG_H

typedef enum log_level {
    LOG_LEVEL_INFO,
    LOG_LEVEL_WARN,
    LOG_LEVEL_ERR,
} log_level_t;

void log_write(log_level_t log_level,
        const char *fname, int lineno, const char *fnname,
        const char *__restrict format, ...);

#endif /* _HYDRA_LIB_LOG_H */
