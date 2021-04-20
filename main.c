#include <stdio.h>
#include "shell/shell.h"
int main(int argc, char** argv) {
    char pool_buffer[1024];
    pool_t pool;
    pool_init(&pool, pool_buffer, sizeof(pool_buffer));
    fs_t* fs = fs_create(&pool);
    shell_t shell;
    char buf[80];
    shell_init(&shell, &fs);
    printf("%d", sizeof(void*));
    while (gets(buf) != EOF) {
        shell_system(&shell, buf);
    }
    return 0;
}