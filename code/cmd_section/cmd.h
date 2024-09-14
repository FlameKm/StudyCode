//
// Created by hyc on 24-9-14.
//

#ifndef CMD_H
#define CMD_H

struct cmd_t
{
    void (*fn)(char *, char *);
    const char *name;
};

#define reflect(_type, _x) _type __cmd_##_x __attribute__((section("cmd"))) = {_x, #_x}
extern cmd_t __start_cmd, __stop_cmd;



#endif // CMD_H