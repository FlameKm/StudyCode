//
// Created by hyc on 24-9-14.
//
#include "cmd.h"

int add(int a, int b)
{
    return a + b;
}
reflect(cmd_t, add);

int del(int a, int b)
{
    return a - b;
}
reflect(cmd_t, del);

int mul(int a, int b)
{
    return a * b;
}
reflect(cmd_t, mul);

int div(int a, int b)
{
    return a / b;
}
reflect(cmd_t, div);
