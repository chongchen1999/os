#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
    pid_t pid1 = fork();

    if (pid1 == 0) {
        printf("Child 1: Running...\n");
        sleep(3);
        return 10;
    }

    pid_t pid2 = fork();
    if (pid2 == 0) {
        printf("Child 2: Running...\n");
        sleep(2);
        return 20;
    }

    // Parent process
    int status;
    printf("Parent: Waiting for Child 2 (PID %d)...\n", pid2);
    waitpid(pid2, &status, 0);  // Wait for specific child

    if (WIFEXITED(status)) {
        printf("Parent: Child 2 exited with status %d\n", WEXITSTATUS(status));
    }

    // Wait for any remaining child
    printf("Parent: Waiting for any other child...\n");
    wait(NULL);
    printf("Parent: Done.\n");

    return 0;
}
