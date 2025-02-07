//
// Created by yuancan on 25-2-7.
//
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/shm.h>
#include "shmdata.h"

int main(int argc, char **argv)
{
    struct shared_data *shared = NULL;

    // 创建共享内存, key 为共享内存标识符
    int shmid = shmget((key_t)1234, sizeof(struct shared_data), 0666 | IPC_CREAT);
    if (shmid == -1) {
        fprintf(stderr, "shmget failed\n");
        exit(EXIT_FAILURE);
    }

    // 启动对共享内存的访问，将共享内存连接到当前的进程地址空间
    void *shmaddr = shmat(shmid, (void *)0, 0);
    if (shmaddr == (void *)-1) {
        fprintf(stderr, "shmat failed\n");
        exit(EXIT_FAILURE);
    }

    printf("\nMemory attached at %p\n", shmaddr);

    // 设置共享内存
    shared = (struct shared_data *)shmaddr;
    while (1)// 向共享内存中写数据
    {
        char buffer[BUFSIZ + 1];
        // 数据还没有被读取，则等待数据被读取，不能向共享内存中写入文本
        while (shared->written == 1) {
            usleep(1000);
        }

        // 向共享内存中写入数据
        printf("Enter some text: ");
        fgets(buffer, BUFSIZ, stdin);
        strncpy(shared->text, buffer, TEXT_SZ);

        // 写完数据，设置written使共享内存段可读
        shared->written = 1;

        // 输入了end，退出循环（程序）
        if (strncmp(buffer, "end", 3) == 0) {
            break;
        }
    }

    // 把共享内存从当前进程中分离
    if (shmdt(shmaddr) == -1) {
        fprintf(stderr, "shmdt failed\n");
        exit(EXIT_FAILURE);
    }

    sleep(2);
    exit(EXIT_SUCCESS);
}