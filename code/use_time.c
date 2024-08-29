//
// Created by FlameKm on 24-7-22.
//

#include <stdio.h>
#include <time.h>
#include <stdint.h>

int main()
{
    volatile int i = 0;
    clock_t tim1 = clock();
    for (i = 0; i < INT32_MAX; i++) {

    }
    clock_t tim2 = clock();
    double time_consume = (double)(tim2 - tim1) / 1000;
    printf("Time consume: %.1f ms\n", time_consume);
}