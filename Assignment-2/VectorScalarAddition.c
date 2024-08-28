#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

#define VECTOR_SIZE 100000000 
#define SAMPLE_SIZE 10        

// Sequential vector-scalar addition
void vector_scalar_addition_sequential(double *vector, double scalar, int size) {
    for (int i = 0; i < size; i++) {
        vector[i] += scalar;
    }
}

// Parallel vector-scalar addition
void vector_scalar_addition_parallel(double *vector, double scalar, int size) {
    #pragma omp parallel for
    for (int i = 0; i < size; i++) {
        vector[i] += scalar;
    }
}

// Function to print a sample of the vector
void print_vector_sample(double *vector, int size, int sample_size) {
    printf("Sample of the vector values after addition:\n");
    for (int i = 0; i < sample_size; i++) {
        printf("vector[%d] = %f\n", i, vector[i]);
    }
}

int main() {
    double *vector = (double *)malloc(VECTOR_SIZE * sizeof(double));
    double scalar = 1;

    // Initialize vector
    for (int i = 0; i < VECTOR_SIZE; i++) {
        vector[i] = (double)i;
    }

    double start_time, end_time, sequential_time, parallel_time, speedup;

    // Sequential Execution
    printf("Sequential Execution\n");
    start_time = omp_get_wtime();
    vector_scalar_addition_sequential(vector, scalar, VECTOR_SIZE);
    end_time = omp_get_wtime();
    sequential_time = end_time - start_time;
    printf("Time taken (sequential): %f seconds\n", sequential_time);

    // Print a sample of the vector values after sequential addition
    print_vector_sample(vector, VECTOR_SIZE, SAMPLE_SIZE);

    // Reinitialize vector for parallel execution
    for (int i = 0; i < VECTOR_SIZE; i++) {
        vector[i] = (double)i;
    }

    // Parallel Execution
    printf("Parallel Execution\n");
    omp_set_num_threads(4);  // Set the number of threads to 4
    start_time = omp_get_wtime();
    vector_scalar_addition_parallel(vector, scalar, VECTOR_SIZE);
    end_time = omp_get_wtime();
    parallel_time = end_time - start_time;
    printf("Time taken (parallel): %f seconds\n", parallel_time);

    // Print a sample of the vector values after parallel addition
    print_vector_sample(vector, VECTOR_SIZE, SAMPLE_SIZE);

    // Speedup calculation
    speedup = sequential_time / parallel_time;
    printf("Speed Up: %f\n", speedup);

    free(vector);
    return 0;
}
