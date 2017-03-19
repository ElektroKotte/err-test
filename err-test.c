#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "err.h"

enum {
    ERR_OK, ERR_FAIL,
};

static void printer(const char *fmt, ...)
{
    va_list args;

    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
}

bool fail(void)
{
    return false;
}

err_t *fun_a(void)
{
    bool result;
    err_t *err;

    result = fail();
    ERR_ASSERT_MSG(err, on_error, result, ERR_FAIL, "Bad wolf was here: %s", "yes");

    err = NULL;

on_error:
    return err;
}

err_t *fun_b(void)
{
    err_t *err;

    err = fun_a();
    ERR_TJUMP(err, on_error, err_is_ok(err));

    err = NULL;

on_error:
    return err;
}

int main(void) 
{
    err_t *err = fun_b();
    err_consume(err, printer);

    return EXIT_SUCCESS;
}
