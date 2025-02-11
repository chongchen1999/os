#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int ready = 0;  // Shared variable

void* waiter(void* arg) {
    pthread_mutex_lock(&mutex);
    while (!ready) {  // Keep checking the condition
        printf("Waiter (Thread ID: %lu): Waiting for signal...\n", pthread_self());
        pthread_cond_wait(&cond, &mutex);  // Release mutex and wait
    }
    printf("Waiter (Thread ID: %lu): Received signal, proceeding...\n", pthread_self());

    pthread_mutex_unlock(&mutex);
    return NULL;
}

void* sender(void* arg) {
    sleep(2);  // Simulate work before sending the signal
    pthread_mutex_lock(&mutex);
    ready = 1;  // Update shared variable
    printf("Sender (Thread ID: %lu): Sending signal...\n", pthread_self());
    pthread_cond_broadcast(&cond);  // Wake up all waiting threads
    pthread_mutex_unlock(&mutex);
    return NULL;
}

int main() {
    pthread_t t0, t1, t2;

    pthread_create(&t1, NULL, waiter, NULL);
    pthread_create(&t2, NULL, waiter, NULL);
    pthread_create(&t0, NULL, sender, NULL);

    pthread_join(t0, NULL);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);

    return 0;
}
