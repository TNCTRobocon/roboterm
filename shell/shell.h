#ifndef __HEADER_GUARD_SHELL_
#define __HEADER_GUARD_SHELL_
#include "fs.h"
typedef struct shell {
    fs_t* fs;
    file_directory_t* root;
} shell_t;


int shell_init(shell_t*,fs_t* fs);
int shell_system(shell_t*, const char* line);
int shell_commands(shell_t*, char** word, size_t size);

#endif