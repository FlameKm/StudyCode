#include "cmd.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * The file doesn't know what command on CMD. It only prints
 * the command for define by the `reflect`.
 */
int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        printf("Usage: %s <cmd> <arg1> <arg2>\n", argv[0]);
        printf("supported cmds: ");
        for (cmd_t *c = &__start_cmd; c < &__stop_cmd; c++)
        {
            printf("%s ", c->name);
        }
        return 1;
    }
    for (cmd_t *c = &__start_cmd; c < &__stop_cmd; c++)
    {
        if (strcmp(c->name, argv[1]) == 0)
        {
            c->fn(argv[2], argv[3]);
        }
    }
}

/*   if (strcmp(id->name, argv[1]) == 0)
        {
            id->fn(argv[2], argv[3]);
        }*/