#include <errno.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

extern char **environ;  // Access to the process's environment variables

int main() {
    char *argv[] = {"/bin/echo", "hello", "world",
                    NULL};  // Arguments for the echo command

    if (fork() == 0) {
        printf("Child process\n");
        printf("PID: %d\n", getpid());
        fflush(stdout);  // Flush the output buffer before calling execve
        if (execve(argv[0], argv, environ) < 0) {
            perror("execve");  // Print an error message if execve fails
            return 1;          // Return a non-zero exit code on failure
        }
    }

    printf("Parent process\n");
    printf("PID: %d\n", getpid());
    fflush(stdout);
    execlp("echo", "echo", "hello", "world", NULL);
    // system("echo hello world");

    return 0;  // This will never be reached if execve succeeds
}
