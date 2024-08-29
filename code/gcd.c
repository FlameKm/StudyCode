#include <stdio.h>

// 实现宏的unity

#define GCD(a, b) ({ \
    int _a = a; \
    int _b = b; \
    while (_b != 0) { \
        int _temp = _b; \
        _b = _a % _b; \
        _a = _temp; \
    } \
    _a; \
})

int main()
{
    int a = 70;
    int b = 100;
    printf("GCD(%d, %d) = %d\n", a, b, GCD(a, b));
    return 0;
}