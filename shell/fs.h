#pragma once
#ifndef __HEADER_GUARD_FS__
#define __HEADER_GUARD_FS__

#include <stddef.h>
#include "file.h"
#include "pool.h"

typedef struct fs {
    pool_t pool;
    file_directory_t* root;
} fs_t;

fs_t* fs_init(fs_t*, void* buffer, size_t);
file_header_t* fs_path(fs_t*, const char** path, size_t);

static inline file_directory_t* fs_root(fs_t* fs) {
    return fs ? fs->root : NULL;
}
#endif