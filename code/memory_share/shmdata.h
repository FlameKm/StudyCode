#ifndef _SHMDATA_H_HEADER
#define _SHMDATA_H_HEADER

#define TEXT_SZ 2048

struct shared_data
{
    int written; // 作为一个标志，非0：表示可读，0：表示可写
    char text[TEXT_SZ]; // 记录写入 和 读取 的文本
};

#endif