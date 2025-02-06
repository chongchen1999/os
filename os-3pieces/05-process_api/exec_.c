#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
    pid_t pid;

    // Fork a child process
    pid = fork();

    if (pid < 0) {
        // Fork failed
        perror("fork failed");
        exit(1);
    } else if (pid == 0) {
        // Child process
        printf("Child Process (PID: %d) executing 'ls -l'\n", getpid());
        execl("/bin/ls", "ls", "-l", (char *)NULL);

        // If exec() fails
        perror("exec failed");
        exit(1);
    } else {
        // Parent process
        printf("Parent Process (PID: %d) waiting for child (PID: %d)\n",
               getpid(), pid);
        wait(NULL);  // Wait for child process to complete
        printf("Parent Process: Child completed.\n");
    }

    return 0;
}
