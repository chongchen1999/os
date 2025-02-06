#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    // Step 1: Create a new child process using fork()
    int rc = fork();
    
    if (rc < 0) {
        // If fork() fails, print an error message and exit
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (rc == 0) {
        // This is the child process
        // Step 2: Close standard output (stdout), so the output doesn't go to the terminal
        close(STDOUT_FILENO);
        
        // Step 3: Open the file "p4.output" for writing. If the file doesn't exist, create it.
        // If it exists, truncate (clear) it before writing new data.
        // File permissions are set to allow the user to read, write, and execute the file.
        open("./p4.output", O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU); 
        
        // Step 4: Prepare arguments for the execvp call. We want to run the "wc" command
        // with the argument "p4.c", which will count the words in the "p4.c" file.
        char *myargs[3];
        myargs[0] = strdup("wc");    // Program name: wc
        myargs[1] = strdup("p4.c");  // Argument: file to count words (p4.c)
        myargs[2] = NULL;            // Null-terminate the arguments array
        
        // Step 5: Replace the current child process with the wc program.
        // This will execute the wc command with the arguments we provided.
        execvp(myargs[0], myargs);
        
        // If execvp fails, it will return here, so we print an error and exit.
        fprintf(stderr, "execvp failed\n");
        exit(1);
    } else {
        // This is the parent process
        // Step 6: Wait for the child process to complete before continuing
        int rc_wait = wait(NULL);  // Parent waits for the child to finish
    }
    
    return 0;  // Program finishes successfully
}
