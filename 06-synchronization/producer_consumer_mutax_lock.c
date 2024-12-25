#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <assert.h>

#define BUFFER_SIZE 10  // Size of the shared buffer

int buffer[BUFFER_SIZE]; // The shared buffer
int count = 0;           // Number of items in the buffer
int item = 0;            // ID of the next item to be produced

pthread_mutex_t mutex;                // Mutex to protect the buffer
pthread_cond_t cond_producer;         // Condition variable for producer
pthread_cond_t cond_consumer;         // Condition variable for consumer

// Struct to pass thread arguments
typedef struct {
    int id; // ID of the producer or consumer
} ThreadArgs;

// Function for the producer thread
void* producer(void* arg) {
    ThreadArgs* args = (ThreadArgs*)arg;
    int producer_id = args->id;

    while (1) {
        pthread_mutex_lock(&mutex);

        // Wait if the buffer is full
        while (count == BUFFER_SIZE) {
            pthread_cond_wait(&cond_producer, &mutex);
        }
        assert(count < BUFFER_SIZE);

        // Produce a new item
        item++;
        buffer[count] = item;
        count++;
        printf("Producer %d produced: %d, total items: %d\n", producer_id, item, count);

        // Signal the consumer that there is an item available
        pthread_cond_signal(&cond_consumer);
        pthread_mutex_unlock(&mutex);

        sleep(0.1); // Simulate production time
    }
    return NULL;
}

// Function for the consumer thread
void* consumer(void* arg) {
    ThreadArgs* args = (ThreadArgs*)arg;
    int consumer_id = args->id;

    while (1) {
        pthread_mutex_lock(&mutex);

        // Wait if the buffer is empty
        while (count == 0) {
            pthread_cond_wait(&cond_consumer, &mutex);
        }
        assert(count > 0);

        // Consume an item
        int consumed_item = buffer[count - 1];
        count--;
        printf("Consumer %d consumed: %d, total items: %d\n", consumer_id, consumed_item, count);

        // Signal the producer that there is space available
        pthread_cond_signal(&cond_producer);
        pthread_mutex_unlock(&mutex);

        sleep(0.2); // Simulate consumption time
    }
    return NULL;
}

// Function to create producer threads
void createProducers(pthread_t* producer_threads, int num_producers) {
    for (int i = 0; i < num_producers; i++) {
        ThreadArgs* args = malloc(sizeof(ThreadArgs));
        args->id = i + 1; // Assign unique ID to each producer
        pthread_create(&producer_threads[i], NULL, producer, args);
    }
}

// Function to create consumer threads
void createConsumers(pthread_t* consumer_threads, int num_consumers) {
    for (int i = 0; i < num_consumers; i++) {
        ThreadArgs* args = malloc(sizeof(ThreadArgs));
        args->id = i + 1; // Assign unique ID to each consumer
        pthread_create(&consumer_threads[i], NULL, consumer, args);
    }
}

int main() {
    int num_producers = 10;
    int num_consumers = 15;

    pthread_t producer_threads[num_producers];
    pthread_t consumer_threads[num_consumers];

    // Initialize the mutex and condition variables
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond_producer, NULL);
    pthread_cond_init(&cond_consumer, NULL);

    // Create the producer and consumer threads
    createProducers(producer_threads, num_producers);
    createConsumers(consumer_threads, num_consumers);

    // Wait for the threads to finish (they won't in this example)
    for (int i = 0; i < num_producers; i++) {
        pthread_join(producer_threads[i], NULL);
    }

    for (int i = 0; i < num_consumers; i++) {
        pthread_join(consumer_threads[i], NULL);
    }

    // Clean up
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond_producer);
    pthread_cond_destroy(&cond_consumer);

    return 0;
}
