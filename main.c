#include <stdio.h>
#include "shell/shell.h"

char fs_buffer[1024];
fs_t fs;
int main(int argc, char** argv) {
    fs_init(&fs, &fs_buffer, sizeof(fs_buffer));

    shell_t shell;
    char buf[80];
    shell_init(&shell, &fs);
    while (gets(buf) != EOF) {
        shell_system(&shell, buf);
    }
    return 0;
}