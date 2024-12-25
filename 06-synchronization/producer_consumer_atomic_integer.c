#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#define BUFFER_SIZE 10

typedef struct {
    int buffer[BUFFER_SIZE];
    int count;
    int in;
    int out;
    pthread_mutex_t mutex;
    pthread_cond_t not_full;    // Condition variable for buffer not full
    pthread_cond_t not_empty;   // Condition variable for buffer not empty
} Buffer;

Buffer shared_buffer = {
    .count = 0,
    .in = 0,
    .out = 0
};

int item = 0;

// Producer function
void* producer(void* arg) {
    int producer_id = *(int*)arg;
    free(arg);  // Free the allocated memory for id

    while (1) {
        pthread_mutex_lock(&shared_buffer.mutex);

        // Wait while buffer is full
        while (shared_buffer.count == BUFFER_SIZE) {
            printf("Producer %d waiting - buffer full\n", producer_id);
            pthread_cond_wait(&shared_buffer.not_full, &shared_buffer.mutex);
        }

        // Produce an item
        item++;
        shared_buffer.buffer[shared_buffer.in] = item;
        shared_buffer.in = (shared_buffer.in + 1) % BUFFER_SIZE;
        shared_buffer.count++;

        printf("Producer %d produced: %d, total items: %d\n", 
               producer_id, item, shared_buffer.count);

        // Signal that buffer is not empty
        pthread_cond_signal(&shared_buffer.not_empty);
        pthread_mutex_unlock(&shared_buffer.mutex);

        // Sleep for random time between 0.1 and 0.5 seconds
        usleep((100 + (rand() % 400)) * 100);
    }
    return NULL;
}

// Consumer function
void* consumer(void* arg) {
    int consumer_id = *(int*)arg;
    free(arg);  // Free the allocated memory for id

    while (1) {
        pthread_mutex_lock(&shared_buffer.mutex);

        // Wait while buffer is empty
        while (shared_buffer.count == 0) {
            printf("Consumer %d waiting - buffer empty\n", consumer_id);
            pthread_cond_wait(&shared_buffer.not_empty, &shared_buffer.mutex);
        }

        // Consume an item
        int item = shared_buffer.buffer[shared_buffer.out];
        shared_buffer.out = (shared_buffer.out + 1) % BUFFER_SIZE;
        shared_buffer.count--;

        printf("Consumer %d consumed: %d, total items: %d\n", 
               consumer_id, item, shared_buffer.count);

        // Signal that buffer is not full
        pthread_cond_signal(&shared_buffer.not_full);
        pthread_mutex_unlock(&shared_buffer.mutex);

        // Sleep for random time between 0.2 and 0.7 seconds
        usleep((200 + (rand() % 500)) * 100);
    }
    return NULL;
}

int main() {
    srand(time(NULL));  // Initialize random seed

    // Initialize synchronization primitives
    pthread_mutex_init(&shared_buffer.mutex, NULL);
    pthread_cond_init(&shared_buffer.not_full, NULL);
    pthread_cond_init(&shared_buffer.not_empty, NULL);

    int num_producers = 10;
    int num_consumers = 15;

    pthread_t producers[num_producers];
    pthread_t consumers[num_consumers];

    // Create producer threads
    for (int i = 0; i < num_producers; i++) {
        int* id = malloc(sizeof(int));
        *id = i + 1;
        if (pthread_create(&producers[i], NULL, producer, id) != 0) {
            perror("Failed to create producer thread");
            exit(1);
        }
    }

    // Create consumer threads
    for (int i = 0; i < num_consumers; i++) {
        int* id = malloc(sizeof(int));
        *id = i + 1;
        if (pthread_create(&consumers[i], NULL, consumer, id) != 0) {
            perror("Failed to create consumer thread");
            exit(1);
        }
    }

    // Wait for user input to terminate
    printf("Press Enter to terminate...\n");
    getchar();

    // Cleanup and exit
    pthread_mutex_destroy(&shared_buffer.mutex);
    pthread_cond_destroy(&shared_buffer.not_full);
    pthread_cond_destroy(&shared_buffer.not_empty);

    return 0;
}