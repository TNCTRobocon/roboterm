#include "fs.h"
#include <stdio.h>
static int cmd_echo(void* ctx, int argc, char** argv) {
    for (size_t i = 1; i < argc; i++) {
        printf("%s ", argv[i]);
    }
    printf("\n");
    return 0;
}

fs_t* fs_create(pool_t* p) {
    fs_t* fs = (fs_t*)pool_malloc(p, sizeof(fs_t));

    file_directory_t* root = file_directory_create(p, "root", 8, 7);
    file_directory_t* bin = file_directory_create(p, "bin", 8, 7);
    file_excute_t* echo = file_excute_create(p, "echo", cmd_echo, NULL);
    file_directory_add(root, (file_header_t*)bin);
    return fs;
}