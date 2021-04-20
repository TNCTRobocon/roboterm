#include "fs.h"
#include <stdio.h>
static int cmd_echo(void* ctx, int argc, char** argv) {
    for (size_t i = 1; i < argc; i++) {
        printf("%s ", argv[i]);
    }
    printf("\n");
    return 0;
}

fs_t* fs_init(fs_t* fs, void* buffer, size_t buffer_size) {
    if (!fs || !buffer) return NULL;
    pool_init(&fs->pool, buffer, buffer_size);

    file_directory_t* root = fs->root =
        file_directory_create(&fs->pool, "root", 8, 7);
    file_directory_t* bin = file_directory_create(&fs->pool, "bin", 8, 7);
    file_excute_t* echo = file_excute_create(&fs->pool, "echo", cmd_echo, NULL);
    file_directory_add(bin, (file_header_t*)echo);
    file_directory_add(root, (file_header_t*)bin);
    return fs;
}

file_header_t* fs_path(fs_t* fs, const char** path, size_t size) {
    if (!fs) return NULL;
    if (size == 0) return (file_header_t*)fs->root;
    file_directory_t* dp = fs->root;
    for (size_t i = 0; i < size - 1; i++) {
        file_header_t* hp = file_directory_find(dp, path[i]);
        if (!hp || hp->type != FileTypeDirectory) return NULL;
        dp = (file_directory_t*)hp;
    }
    return file_directory_find(dp, path[size - 1]);
}