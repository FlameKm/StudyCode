//
// Created by hyc on 24-9-14.
//
#include "cmd.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void add(char *a, char *b)
{
    printf("%d\n", atoi(a) + atoi(b));
}
reflect(cmd_t, add);

void del(char *a, char *b)
{
    printf("%d\n", atoi(a) - atoi(b));
}
reflect(cmd_t, del);

void mul(char *a, char *b)
{
    printf("%d\n", atoi(a) * atoi(b));
}
reflect(cmd_t, mul);

void div(char *a, char *b)
{
    printf("%d\n", atoi(a) / atoi(b));
}
reflect(cmd_t, div);