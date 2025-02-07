#include <unistd.h>
#include <stdio.h>

int main() {
    size_t size = 1024; // Allocate 1KB

    // Get the current program break
    void *current_brk = sbrk(0);
    printf("Current program break: %p\n", current_brk);

    // Allocate memory
    void *new_brk = sbrk(size);
    if (new_brk == (void *)-1) {
        perror("sbrk failed");
        return 1;
    }

    printf("New program break: %p\n", sbrk(0));

    // Use the memory
    int *arr = (int *)current_brk;
    arr[0] = 42;

    printf("Value at arr[0]: %d\n", arr[0]);

    // Deallocate memory (optional but uncommon for `sbrk`)
    if (sbrk(-size) == (void *)-1) {
        perror("sbrk failed to shrink");
        return 1;
    }

    return 0;
}
