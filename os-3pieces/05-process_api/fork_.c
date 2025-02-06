#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
    int var = 10;  // A variable in memory
    pid_t pid = fork();

    if (pid > 0) {
        // Parent process
        printf("Parent: var before modification = %d, address = %p\n", var,
               &var);
        var = 20;  // Modify variable
        printf("Parent: var after modification = %d, address = %p\n", var,
               &var);
        wait(NULL);  // Wait for child to finish
    } else if (pid == 0) {
        // Child process
        printf("Child: var before modification = %d, address = %p\n", var,
               &var);
        var = 50;  // Modify variable
        printf("Child: var after modification = %d, address = %p\n", var, &var);
    } else {
        printf("Fork failed!\n");
    }

    return 0;
}
