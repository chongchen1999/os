#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define NPROC 1000
#define MB 128
#define SIZE (MB * (1 << 20))

#define xstr(s) str(s)
#define str(s) #s

int main() {
    char *data = malloc(SIZE);  // 128MB shared memory
    memset(data, '_', SIZE);

    for (int i = 0; i < NPROC - 1; i++) {
        if (fork() == 0) { break; }
    }

    // NPROC processes go here

    asm volatile(".fill 1048576 * " xstr(MB) ", 1, 0x90");  // 128MB shared code

    unsigned int idx = 0;
    int fd = open("/dev/urandom", O_RDONLY);
    assert(fd > 0);
    read(fd, &idx, sizeof(idx));
    close(fd);
    idx %= 1048576 * MB;

    data[idx] = '.';
    printf("pid = %d, write data[%u]\n", getpid(), idx);

    while (1) {
        sleep(1);  // not terminate
    }
}
