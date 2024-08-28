#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <unistd.h>
#include <time.h>

#define BUFFER_SIZE 10
#define NUM_PRODUCERS 2
#define NUM_CONSUMERS 2
#define NUM_ITEMS 40

int buffer[BUFFER_SIZE];
int in = 0;
int out = 0;
int count = 0;

omp_lock_t mutex;

void init_locks() {
    omp_init_lock(&mutex);
    
}

void destroy_locks() {
    omp_destroy_lock(&mutex);
}

void produce(int id) {
    int item = rand() % 100;

    while (1) {
        omp_set_lock(&mutex);
        if (count < BUFFER_SIZE) {
            buffer[in] = item;
            in = (in + 1) % BUFFER_SIZE;
            count++;
            omp_unset_lock(&mutex);
            // printf("Producer %d produced item %d (count: %d)\n", id, item, count);
            break;
        }
        omp_unset_lock(&mutex);
    }
}


int consume(int id) {
    int item = -1;

    while (1) {
        omp_set_lock(&mutex);
        if (count > 0) {
            item = buffer[out];
            out = (out + 1) % BUFFER_SIZE;
            count--;
            omp_unset_lock(&mutex);
            // printf("Consumer %d consumed item %d (count: %d)\n", id, item, count);
            break;
        }
        omp_unset_lock(&mutex);
    }

    return item;
}

void producer(int id) {
    for (int i = 0; i < NUM_ITEMS / NUM_PRODUCERS; i++) {
        produce(id);
        usleep(rand() % 100000);  // Sleep for up to 0.1 seconds
    }
}

void consumer(int id) {
    for (int i = 0; i < NUM_ITEMS / NUM_CONSUMERS; i++) {
        consume(id);
        usleep(rand() % 100000);  // Sleep for up to 0.1 seconds
    }
}

void sequential_producer_consumer() {
    for (int i = 0; i < NUM_ITEMS; i++) {
        int id = i % (NUM_PRODUCERS + NUM_CONSUMERS);
        if (id < NUM_PRODUCERS) {
            produce(id);
        } else {
            consume(id - NUM_PRODUCERS);
        }
    }
}

int main() {
    double sequential_time = 0;  // Placeholder for actual sequential time
    srand(time(NULL));
    init_locks();

    //Sequential Execution
    printf("Sequential Execution\n");
    clock_t start_time = clock();

    sequential_producer_consumer();

    clock_t end_time = clock();
    sequential_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    printf("Time taken (sequential): %f seconds\n", sequential_time);


    // Parallel Execution
    printf("Parallel Execution\n");
    double parallel_start_time = omp_get_wtime();

    #pragma omp parallel sections num_threads(NUM_PRODUCERS + NUM_CONSUMERS)
    {
        #pragma omp section
        {
            producer(0);
        }
        #pragma omp section
        {
            producer(1);
        }
        #pragma omp section
        {
            consumer(0);
        }
        #pragma omp section
        {
            consumer(1);
        }
    }

    double parallel_end_time = omp_get_wtime();
    double parallel_time = parallel_end_time - parallel_start_time;
    printf("Time taken (parallel): %f seconds\n", parallel_time);

    destroy_locks();

    // Speedup calculation (requires sequential timing code to be integrated into this program)
    double speedup = sequential_time / parallel_time;
    printf("Speed Up: %f\n", speedup);

    return 0;
}
