#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

// Shared variables
bool flag[2] = {
    false, false};  // Each process signals intent to enter critical section
int turn = 0;       // Indicates whose turn it is to enter the critical section

// Function for Process 0
void* process_0(void* arg) {
    for (int i = 0; i < 5; i++) {  // Repeat 5 times
        // Entry Section
        flag[0] = true;  // Process 0 signals intent to enter
        turn = 1;        // Give priority to Process 1
        while (flag[1] && turn == 1) {
            // Busy waiting
        }

        // Critical Section
        printf("Process 0: In critical section (iteration %d)\n", i);
        sleep(1);  // Simulate work in critical section

        // Exit Section
        flag[0] = false;  // Process 0 exits critical section

        // Remainder Section
        printf("Process 0: In remainder section\n");
        sleep(1);  // Simulate other work
    }
    return NULL;
}

// Function for Process 1
void* process_1(void* arg) {
    for (int i = 0; i < 5; i++) {  // Repeat 5 times
        // Entry Section
        flag[1] = true;  // Process 1 signals intent to enter
        turn = 0;        // Give priority to Process 0
        while (flag[0] && turn == 0) {
            // Busy waiting
        }

        // Critical Section
        printf("Process 1: In critical section (iteration %d)\n", i);
        sleep(1);  // Simulate work in critical section

        // Exit Section
        flag[1] = false;  // Process 1 exits critical section

        // Remainder Section
        printf("Process 1: In remainder section\n");
        sleep(1);  // Simulate other work
    }
    return NULL;
}

int main() {
    pthread_t thread_0, thread_1;

    // Create threads for both processes
    pthread_create(&thread_0, NULL, process_0, NULL);
    pthread_create(&thread_1, NULL, process_1, NULL);

    // Wait for both threads to finish
    pthread_join(thread_0, NULL);
    pthread_join(thread_1, NULL);

    return 0;
}
