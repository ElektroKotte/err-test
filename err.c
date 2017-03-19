#define _GNU_SOURCE

#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "err.h"

#define ERR_INC 32

err_t *err_new(int code)
{
    err_t *err = malloc(sizeof(err_t));
    if (!err) return NULL;

    err->code = code;
    err->size = 0;
    err->capacity = 0;
    err->trace = NULL;

    return err;
}

err_t *err_push(err_t *err, const char *file, const char *func, unsigned line, const char *msgfmt, ...)
{
    trace_t entry = {
        .file = file,
        .func = func,
        .line = line,
        .message = NULL,
    };

    if (!err) return NULL;

    if (err->size >= err->capacity) {
        size_t new_cap = err->capacity + ERR_INC;
        trace_t *tmp = realloc(err->trace, new_cap * sizeof(trace_t));
        if (!tmp) return NULL;

        err->trace = tmp;
        err->capacity = new_cap;
    }

    if (msgfmt) {
        va_list args;
        char *msg;

        va_start(args, msgfmt);
        vasprintf(&msg, msgfmt, args);
        va_end(args);
        entry.message = msg;
    }

    err->trace[err->size] = entry;
    err->size += 1;

    return err;
}

void err_consume(err_t *err, void (*printer)(const char *fmt, ...))
{
    if (!err) return;

    for (size_t i = 0; i < err->size; i++) {
        trace_t *t = &err->trace[i];
        if (t->message) {
            printer("%02zd: %s:%u in %s() - %s\n", i, t->file, t->line, t->func, t->message);
            free(t->message);
        } else {
            printer("%02zd: %s:%u %s()\n", i, t->file, t->line, t->func);
        }
    }

    free(err->trace);
    free(err);
}

bool err_is_ok(const err_t *err)
{
    if (!err) return true;
    return err->code == 0;
}
