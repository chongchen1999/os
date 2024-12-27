#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/syscall.h>
#include <time.h>
#include <unistd.h>

void func() { printf("Goodbye, Cruel OS World!\n"); }

int main(int argc, char *argv[], char *envp[]) {
    char **p = envp;
    while (p && *p) {
        printf("%s\n", *p);
        p++;
    }
    atexit(func);

    if (argc < 2) return EXIT_FAILURE;

    if (strcmp(argv[1], "exit") == 0) exit(0);
    if (strcmp(argv[1], "_exit") == 0) _exit(0);
    if (strcmp(argv[1], "__exit") == 0) syscall(SYS_exit, 0);
}