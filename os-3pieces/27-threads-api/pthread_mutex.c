#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

pthread_mutex_t lock;  // Declare a mutex

void *thread_function(void *arg) {
    pthread_mutex_lock(&lock);  // Lock the mutex
    printf("Thread %ld has entered the critical section.\n", (long)arg);

    // Critical section
    sleep(1);

    printf("Thread %ld is leaving the critical section.\n", (long)arg);
    pthread_mutex_unlock(&lock);  // Unlock the mutex

    return NULL;
}

int main() {
    printf("size of pthread_mutex_t is: %lu\n", sizeof(pthread_mutex_t));

    pthread_t t1, t2;

    pthread_mutex_init(&lock, NULL);  // Initialize the mutex

    pthread_create(&t1, NULL, thread_function, (void *)1);
    pthread_create(&t2, NULL, thread_function, (void *)2);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    pthread_mutex_destroy(&lock);  // Destroy the mutex

    return 0;
}
