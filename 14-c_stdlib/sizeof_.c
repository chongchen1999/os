#include <stdio.h>
#include <stdint.h>

int main() {
    printf("Size of int is %lu bytes\n", sizeof(int));
    printf("Size of long is %lu bytes\n", sizeof(long));

    int64_t a = 10;
    printf("Size of a is %lu bytes\n", sizeof(a));
    printf("Value of a is %ld\n", a);

    return 0;
}