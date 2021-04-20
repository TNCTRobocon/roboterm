#pragma once
#ifndef __HEADER_GUARD_FS__
#define __HEADER_GUARD_FS__

#include <stddef.h>
#include "file.h"
#include "pool.h"

typedef struct fs {
    pool_t* pool;
    file_directory_t* root;
} fs_t;

fs_t* fs_create(pool_t* p);
file_header_t* fs_get_file(fs_t*, const char* path);

static inline file_directory_t* fs_root(fs_t* fs) {
    return fs ? fs->root : NULL;
}
#endif