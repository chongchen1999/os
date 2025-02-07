#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>

int main() {
    size_t size = 4096;  // Allocate 4KB

    // Allocate memory
    void *addr = mmap(NULL, size, PROT_READ | PROT_WRITE,
                      MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (addr == MAP_FAILED) {
        perror("mmap failed");
        return 1;
    }

    // Use the memory
    int *arr = (int *)addr;
    arr[0] = 42;

    printf("Value at arr[0]: %d\n", arr[0]);

    // Free the memory
    if (munmap(addr, size) == -1) {
        perror("munmap failed");
        return 1;
    }

    return 0;
}
