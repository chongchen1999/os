#include <fcntl.h>
#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>

int main() {
    int fd = open("../example.txt", O_RDONLY);
    if (fd < 0) {
        perror("open");
        return 1;
    }

    size_t length = 4096;  // Map first 4KB of the file
    char *mapped = mmap(NULL, length, PROT_READ, MAP_PRIVATE, fd, 0);
    if (mapped == MAP_FAILED) {
        perror("mmap");
        close(fd);
        return 1;
    }

    // Access the mapped file content
    printf("File content: %.50s\n", mapped);

    // Unmap and clean up
    munmap(mapped, length);
    close(fd);

    return 0;
}
