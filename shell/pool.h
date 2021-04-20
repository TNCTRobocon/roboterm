#pragma once
#ifndef __HEADER_GUARD_POOL__
#define __HEADER_GUARD_POOL__
#include <stddef.h>

typedef struct pool {
    size_t used, size;
    char* mem;
} pool_t;

//開放できない系、malloc
pool_t* pool_init(pool_t*, void* mem, size_t);
void* pool_malloc(pool_t*, size_t);

static inline size_t pool_used(const pool_t* pp) {
    return pp ? pp->used : 0;
}
static inline size_t pool_size(const pool_t* pp) {
    return pp ? pp->size : 0;
}
#endif