#include "cmd.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define _noreturn __attribute__((noreturn))

/*
 * The file doesn't know what command on CMD. It only prints
 * the command for define by the `reflect`.
 */
void _noreturn usage(char *argv[])
{
    printf("Usage: %s <cmd> <arg1> <arg2>\n", argv[0]);
    printf("supported cmds: ");
    for (cmd_t *c = &__start_cmd; c < &__stop_cmd; c++) {
        printf("%s ", c->name);
    }
    exit(1);
}

int main(int argc, char *argv[])
{
    if (argc != 4) {
        usage(argv);
    }

    cmd_t *c;
    for (c = &__start_cmd; c < &__stop_cmd; c++) {
        if (strcmp(c->name, argv[1]) == 0) {
            int res = c->fn(atoi(argv[2]), atoi(argv[3]));
            printf("%d\n", res);
            break;
        }
    }

    if (c == &__stop_cmd) {
        usage(argv);
    }
}
