#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_PHILOSOPHERS 5

// Mutex for each fork
pthread_mutex_t forks[NUM_PHILOSOPHERS];

// Philosopher routine
void* philosopher(void* arg) {
    int id = *(int*)arg;
    int left_fork = id;
    int right_fork = (id + 1) % NUM_PHILOSOPHERS;

    while (1) {
        // Thinking
        printf("Philosopher %d is thinking.\n", id);
        sleep(1);

        // Picking up forks (avoid deadlock with ordering)
        if (id % 2 == 0) {
            pthread_mutex_lock(&forks[left_fork]);
            printf("Philosopher %d picked up left fork %d.\n", id, left_fork);
            pthread_mutex_lock(&forks[right_fork]);
            printf("Philosopher %d picked up right fork %d.\n", id, right_fork);
        } else {
            pthread_mutex_lock(&forks[right_fork]);
            printf("Philosopher %d picked up right fork %d.\n", id, right_fork);
            pthread_mutex_lock(&forks[left_fork]);
            printf("Philosopher %d picked up left fork %d.\n", id, left_fork);
        }

        // Eating
        printf("Philosopher %d is eating.\n", id);
        sleep(2);

        // Putting down forks
        pthread_mutex_unlock(&forks[left_fork]);
        printf("Philosopher %d put down left fork %d.\n", id, left_fork);
        pthread_mutex_unlock(&forks[right_fork]);
        printf("Philosopher %d put down right fork %d.\n", id, right_fork);
    }

    return NULL;
}

int main() {
    pthread_t threads[NUM_PHILOSOPHERS];
    int philosopher_ids[NUM_PHILOSOPHERS];

    // Initialize mutexes
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_mutex_init(&forks[i], NULL);
    }

    // Create philosopher threads
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        philosopher_ids[i] = i;
        pthread_create(&threads[i], NULL, philosopher, &philosopher_ids[i]);
    }

    // Join threads (optional, depending on the application)
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Destroy mutexes
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_mutex_destroy(&forks[i]);
    }

    return 0;
}
