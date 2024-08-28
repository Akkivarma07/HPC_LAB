#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define NUM_STEPS 1000000000 // Adjust this for different data sizes

// Sequential Pi calculation
double calculate_pi_sequential(long long num_steps) {
    double pi = 0.0;
    double step = 1.0 / (double)num_steps;
    for (long long i = 0; i < num_steps; i++) {
        double x = (i + 0.5) * step;
        pi += 4.0 / (1.0 + x * x);
    }
    pi *= step;
    return pi;
}

// Parallel Pi calculation
double calculate_pi_parallel(long long num_steps) {
    double pi = 0.0;
    double step = 1.0 / (double)num_steps;

    #pragma omp parallel
    {
        double x, sum = 0.0;
        #pragma omp for
        for (long long i = 0; i < num_steps; i++) {
            x = (i + 0.5) * step;
            sum += 4.0 / (1.0 + x * x);
        }
        #pragma omp atomic
        pi += sum;
    }

    pi *= step;
    return pi;
}

int main() {
    double sequential_start_time, sequential_end_time, parallel_start_time, parallel_end_time;
    double pi_sequential, pi_parallel;
    double speedup;

    // Sequential Execution
    printf("Sequential Execution\n");
    sequential_start_time = omp_get_wtime();
    pi_sequential = calculate_pi_sequential(NUM_STEPS);
    sequential_end_time = omp_get_wtime();
    double sequential_time = sequential_end_time - sequential_start_time;
    printf("Pi: %.15f, Time taken (sequential): %f seconds\n", pi_sequential, sequential_time);

    // Parallel Execution
    printf("Parallel Execution\n");
    omp_set_num_threads(4); // Set the number of threads
    parallel_start_time = omp_get_wtime();
    pi_parallel = calculate_pi_parallel(NUM_STEPS);
    parallel_end_time = omp_get_wtime();
    double parallel_time = parallel_end_time - parallel_start_time;
    printf("Pi: %.15f, Time taken (parallel): %f seconds\n", pi_parallel, parallel_time);

    // Speedup calculation
    speedup = sequential_time / parallel_time;
    printf("Speed Up: %f\n", speedup);

    return 0;
}
