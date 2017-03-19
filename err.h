#pragma once

#include <stdbool.h>
#include <stddef.h>

typedef struct {
    const char *file;
    const char *func;
    unsigned line;
    char *message;
} trace_t;

typedef struct {
    int code;
    trace_t *trace;
    size_t size;
    size_t capacity;
} err_t;

#define ERR_ASSERT_MSG(var, handler, cond, code, msgfmt, ...)                                   \
    do {                                                                                        \
        if (!(cond)) {                                                                          \
            var = err_push(err_new(code), __FILE__, __func__, __LINE__, msgfmt, ##__VA_ARGS__); \
            goto handler;                                                                       \
        }                                                                                       \
    } while (0)

#define ERR_ASSERT(var, handler, cond, code) ERR_ASSERT_MSG(var, handler, cond, code, NULL)

#define ERR_TJUMP_MSG(var, handler, cond, msgfmt, ...)                          \
    do {                                                                        \
        if (!(cond)) {                                                          \
            err_push(var, __FILE__, __func__, __LINE__, msgfmt, ##__VA_ARGS__); \
            goto handler;                                                       \
        }                                                                       \
    } while (0)

#define ERR_TJUMP(var, handler, cond) ERR_TJUMP_MSG(var, handler, cond, NULL)

err_t *err_new(int code);
err_t *err_push(err_t *err, const char *file, const char *func, unsigned line, const char *fmt,
                ...);
void err_consume(err_t *err, void (*printer)(const char *fmt, ...));
bool err_is_ok(const err_t *err);
