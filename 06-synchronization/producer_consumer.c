#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define BUFFER_SIZE 10  // Size of the shared buffer

int buffer[BUFFER_SIZE]; // The shared buffer
int count = 0;           // Number of items in the buffer

pthread_mutex_t mutex;                // Mutex to protect the buffer
pthread_cond_t cond_producer;         // Condition variable for producer
pthread_cond_t cond_consumer;         // Condition variable for consumer

// Function for the producer thread
void* producer(void* arg) {
    while (1) {
        int item = rand() % 100; // Produce a random item

        pthread_mutex_lock(&mutex);

        // Wait if the buffer is full
        while (count == BUFFER_SIZE) {
            pthread_cond_wait(&cond_producer, &mutex);
        }

        // Add the item to the buffer
        buffer[count] = item;
        count++;
        printf("Producer produced: %d, total items: %d\n", item, count);

        // Signal the consumer that there is an item available
        pthread_cond_signal(&cond_consumer);
        pthread_mutex_unlock(&mutex);

        sleep(1); // Simulate production time
    }
    return NULL;
}

// Function for the consumer thread
void* consumer(void* arg) {
    while (1) {
        pthread_mutex_lock(&mutex);

        // Wait if the buffer is empty
        while (count == 0) {
            pthread_cond_wait(&cond_consumer, &mutex);
        }

        // Remove an item from the buffer
        int item = buffer[count - 1];
        count--;
        printf("Consumer consumed: %d, total items: %d\n", item, count);

        // Signal the producer that there is space available
        pthread_cond_signal(&cond_producer);
        pthread_mutex_unlock(&mutex);

        sleep(2); // Simulate consumption time
    }
    return NULL;
}

int main() {
    pthread_t producer_thread, consumer_thread;

    // Initialize the mutex and condition variables
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond_producer, NULL);
    pthread_cond_init(&cond_consumer, NULL);

    // Create the producer and consumer threads
    pthread_create(&producer_thread, NULL, producer, NULL);
    pthread_create(&consumer_thread, NULL, consumer, NULL);

    // Wait for the threads to finish (they won't in this example)
    pthread_join(producer_thread, NULL);
    pthread_join(consumer_thread, NULL);

    // Clean up
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond_producer);
    pthread_cond_destroy(&cond_consumer);

    return 0;
}
