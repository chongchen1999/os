#include <pthread.h>
#include <stdatomic.h>  // For atomic operations
#include <stdio.h>

typedef struct {
    atomic_flag flag;  // Spin lock flag
} SpinLock;

// Initialize the spin lock
void spin_lock_init(SpinLock *lock) { atomic_flag_clear(&lock->flag); }

// Acquire the spin lock
void spin_lock_acquire(SpinLock *lock) {
    while (atomic_flag_test_and_set(&lock->flag)) {
        // Spin-wait (busy wait) until the lock is available
    }
}

// Release the spin lock
void spin_lock_release(SpinLock *lock) { atomic_flag_clear(&lock->flag); }

// Shared resource
int shared_counter = 0;

// Spin lock instance
SpinLock lock;

// Thread function
void *increment_counter(void *arg) {
    for (int i = 0; i < 1000000; i++) {
        // Acquire the lock
        spin_lock_acquire(&lock);

        // Critical section
        shared_counter++;

        // Release the lock
        spin_lock_release(&lock);
    }
    return NULL;
}

int main() {
    pthread_t thread1, thread2;

    // Initialize the spin lock
    spin_lock_init(&lock);

    // Create two threads
    pthread_create(&thread1, NULL, increment_counter, NULL);
    pthread_create(&thread2, NULL, increment_counter, NULL);

    // Wait for both threads to finish
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    // Print the final value of the shared counter
    printf("Final value of shared_counter: %d\n", shared_counter);

    return 0;
}
