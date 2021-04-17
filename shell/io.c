#include "io.h"

static putc_t system_putc;
static getc_t system_getc;
static flush_t system_flush;

int io_init(putc_t p, getc_t g, flush_t f) {
    if (p == NULL || g == NULL) return -1;
    system_putc = p;
    system_getc = g;
    system_flush = f;
    return 0;
}