#include <assert.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFER_SIZE 10  // Size of the shared buffer

int buffer[BUFFER_SIZE];  // The shared buffer
int in = 0;               // Index for the producer
int out = 0;              // Index for the consumer
int item = 0;             // Item to be produced

// Semaphores
sem_t empty;  // Tracks empty slots
sem_t full;   // Tracks filled slots
sem_t mutex;  // Ensures mutual exclusion

// Struct to pass thread arguments
typedef struct {
    int id;  // ID of the producer or consumer
} ThreadArgs;

// Function for the producer thread
void* producer(void* arg) {
    ThreadArgs* args = (ThreadArgs*)arg;
    int producer_id = args->id;

    while (1) {
        // Produce a new item
        item++;

        // Wait for an empty slot
        sem_wait(&empty);

        // Lock the critical section
        sem_wait(&mutex);

        // Add the item to the buffer
        buffer[in] = item;
        printf("Producer %d produced: %d at index %d\n", producer_id, item, in);
        in = (in + 1) % BUFFER_SIZE;

        // Unlock the critical section
        sem_post(&mutex);

        // Signal that a new item is available
        sem_post(&full);

        sleep(0.1);  // Simulate production time
    }
    return NULL;
}

// Function for the consumer thread
void* consumer(void* arg) {
    ThreadArgs* args = (ThreadArgs*)arg;
    int consumer_id = args->id;

    while (1) {
        // Wait for a filled slot
        sem_wait(&full);

        // Lock the critical section
        sem_wait(&mutex);

        // Remove an item from the buffer
        int consumed_item = buffer[out];
        printf("Consumer %d consumed: %d from index %d\n", consumer_id,
               consumed_item, out);
        out = (out + 1) % BUFFER_SIZE;

        // Unlock the critical section
        sem_post(&mutex);

        // Signal that an empty slot is available
        sem_post(&empty);

        sleep(0.2);  // Simulate consumption time
    }
    return NULL;
}

// Function to create producer threads
void createProducers(pthread_t* producer_threads, int num_producers) {
    for (int i = 0; i < num_producers; i++) {
        ThreadArgs* args = malloc(sizeof(ThreadArgs));
        args->id = i + 1;  // Assign unique ID to each producer
        pthread_create(&producer_threads[i], NULL, producer, args);
    }
}

// Function to create consumer threads
void createConsumers(pthread_t* consumer_threads, int num_consumers) {
    for (int i = 0; i < num_consumers; i++) {
        ThreadArgs* args = malloc(sizeof(ThreadArgs));
        args->id = i + 1;  // Assign unique ID to each consumer
        pthread_create(&consumer_threads[i], NULL, consumer, args);
    }
}

int main() {
    int num_producers = 10;
    int num_consumers = 15;

    pthread_t producer_threads[num_producers];
    pthread_t consumer_threads[num_consumers];

    // Initialize the semaphores
    sem_init(&empty, 0, BUFFER_SIZE);  // Start with all slots empty
    sem_init(&full, 0, 0);             // Start with no filled slots
    sem_init(&mutex, 0, 1);            // Mutex starts unlocked

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
    sem_destroy(&empty);
    sem_destroy(&full);
    sem_destroy(&mutex);

    return 0;
}
