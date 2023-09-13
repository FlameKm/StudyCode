
#include <stdio.h>
void quickSort(int arr[], int start, int end)
{
    if (start >= end) {
        return;
    }
    int mid = arr[start];
    int l = start, r = end;
    while (l < r) {
        while (arr[r] > mid && l < r)
            r--;
        if (l < r)
            arr[l++] = arr[r];
        while (arr[l] < mid && l < r)
            l++;
        if (l < r)
            arr[r--] = arr[l];
    }
    arr[l] = mid;
    quickSort(arr, start, l - 1);
    quickSort(arr, r + 1, end);
}
int main()
{
    int arr[] = {1, 5, 9, 3, 5, 7, 6, 5, 2, 1, 5, 8};
    printf("Before:");
    for (int i = 0; i < sizeof(arr) / sizeof(arr[0]); i++) {
        printf("%d ", arr[i]);
    }
    putchar('\n');

    quickSort(arr, 0, sizeof(arr) / sizeof(arr[0]) - 1);
    printf("After :");
    for (int i = 0; i < sizeof(arr) / sizeof(arr[0]); i++) {
        printf("%d ", arr[i]);
    }
    putchar('\n');
}