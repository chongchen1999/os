#include <linux/futex.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <pthread.h>

// Futex syscall wrapper
int futex(int *uaddr, int futex_op, int val, const struct timespec *timeout, int *uaddr2, int val3) {
    return syscall(SYS_futex, uaddr, futex_op, val, timeout, uaddr2, val3);
}

// Shared futex variable
atomic_int futex_var = 0;

// Thread function
void* thread_func(void* arg) {
    printf("Thread %ld waiting for futex...\n", pthread_self());

    // Wait until futex_var becomes non-zero
    while (atomic_load(&futex_var) == 0) {
        futex((int *)&futex_var, FUTEX_WAIT, 0, NULL, NULL, 0);  // Cast to int *
    }

    printf("Thread %ld woke up!\n", pthread_self());
    return NULL;
}

int main() {
    pthread_t thread;

    // Create a thread
    if (pthread_create(&thread, NULL, thread_func, NULL) != 0) {
        perror("Failed to create thread");
        return EXIT_FAILURE;
    }

    sleep(2); // Simulate work in the main thread

    printf("Main thread signaling futex...\n");
    atomic_store(&futex_var, 1);

    // Wake up the waiting thread
    futex((int *)&futex_var, FUTEX_WAKE, 1, NULL, NULL, 0);  // Cast to int *

    // Wait for the thread to finish
    pthread_join(thread, NULL);

    printf("Main thread done.\n");
    return EXIT_SUCCESS;
}
