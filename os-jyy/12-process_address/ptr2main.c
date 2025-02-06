#include <stdio.h>

int main() {
    unsigned* p;
    p = (void*)main;
    printf("Address of main: %p\n", p);
    return 0;
}