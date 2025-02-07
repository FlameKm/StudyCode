//
// Created by yuancan on 25-2-7.
//
#include <stddef.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "shmdata.h"

int main(int argc, char **argv)
{
    // 创建共享内存, key 为共享内存标识符
    int shmid = shmget((key_t)1234, sizeof(struct shared_data), 0666 | IPC_CREAT);
    if (shmid == -1) {
        fprintf(stderr, "shmat failed\n");
        exit(EXIT_FAILURE);
    }

    // 启动对共享内存的访问，将共享内存连接到当前的进程地址空间
    void *shmaddr = shmat(shmid, 0, 0);
    if (shmaddr == (void *)-1) {
        fprintf(stderr, "shmat failed\n");
        exit(EXIT_FAILURE);
    }

    printf("\nMemory attached at %p\n", shmaddr);

    // 设置共享内存
    struct shared_data *shared = (struct shared_data *)shmaddr;
    shared->written = 0;
    while (1)// 读取共享内存中的数据
    {
        // 没有进程向内存写数据，有数据可读取
        if (shared->written == 1) {
            printf("You wrote: %s", shared->text);

            // 读取完数据，恢复可写
            shared->written = 0;

            if (strncmp(shared->text, "end", 3) == 0) {
                break;
            }
        }
        else {// 有其他进程在写数据，不能读取数据
            usleep(1000);
        }
    }

    // 把共享内存从当前进程中分离
    if (shmdt(shmaddr) == -1) {
        fprintf(stderr, "shmdt failed\n");
        exit(EXIT_FAILURE);
    }

    // 删除共享内存
    if (shmctl(shmid, IPC_RMID, 0) == -1) {
        fprintf(stderr, "shmctl(IPC_RMID) failed\n");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}