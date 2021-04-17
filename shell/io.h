#ifndef __HERADER_GUARD__PIPE__
#define __HERADER_GUARD__PIPE__
#include <stddef.h>
#include "config.h"

typedef int (*putc_t)(char);
typedef int (*flush_t)(void);
typedef char (*getc_t)(void);

int io_init(putc_t, getc_t, flush_t);  //入出力の管理を行う。

int io_putc(char);
void io_puts(const char*);
// void io_printf(const char* fmt, ...);
int io_flush();

#endif