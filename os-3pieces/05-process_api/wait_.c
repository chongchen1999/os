#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
    pid_t pid = fork();

    if (pid > 0) {
        // Parent process
        int status;
        printf("Parent: Waiting for child to finish...\n");
        wait(&status);  // Blocks until any child process terminates

        if (WIFEXITED(status)) {
            printf("Parent: Child exited with status %d\n",
                   WEXITSTATUS(status));
        }
    } else if (pid == 0) {
        // Child process
        printf("Child: Running and then exiting...\n");
        sleep(2);   // Simulate work
        return 42;  // Exit with status 42
    } else {
        printf("Fork failed!\n");
    }

    return 0;
}
