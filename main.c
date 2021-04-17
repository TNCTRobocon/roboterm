#include <stdio.h>
#include "shell/shell.h"

int main(int argc, char** argv) {
    char buf[80];
    shell_t shell;

    shell_init(&shell);

    while (gets(buf) != EOF) {
        shell_system(&shell, buf);
    }
    return 0;
}