    #include <stdio.h>
    #include <string.h>
    #include <stdlib.h>
    #include <omp.h>
    #include <time.h>

    #define VECTOR_SIZE 200
    #define SCALAR 5

    void print_vector(float *vector, int size) {
        for (int i = 0; i < size; i++) {
            printf("%.2f ", vector[i]);
        }
        printf("\n");
    }

    void vector_scalar_add(float *result, float *vector, float scalar, int size, int chunk_size, const char* schedule_type) {
        if (strcmp(schedule_type, "static") == 0) {
            #pragma omp parallel for schedule(static, chunk_size)
            for (int i = 0; i < size; i++) {
                result[i] = vector[i] + scalar;
            }
        } else if (strcmp(schedule_type, "dynamic") == 0) {
            #pragma omp parallel for schedule(dynamic, chunk_size)
            for (int i = 0; i < size; i++) {
                result[i] = vector[i] + scalar;
            }
        }
    }

    double measure_time(float *result, float *vector, float scalar, int size, int chunk_size, const char* schedule_type) {
        double start_time = omp_get_wtime();
        vector_scalar_add(result, vector, scalar, size, chunk_size, schedule_type);
        double end_time = omp_get_wtime();
        return end_time - start_time;
    }

    int main() {
        long long int sequential_product = 0;  // long long int to store large sums
        long long int parallel_product = 0;    // long long int to store large sums
        double static_execution_time;
        double dynamic_execution_time;
        double nowait_execution_time;

        float *vector = (float *)malloc(VECTOR_SIZE * sizeof(float));
        float *result = (float *)malloc(VECTOR_SIZE * sizeof(float));

        srand(time(NULL));
        for (int i = 0; i < VECTOR_SIZE; i++) {
            vector[i] = (float)rand() / RAND_MAX;
        }

        printf("Original vector:\n");
        print_vector(vector, VECTOR_SIZE);

        /****************************************************************************************/
        clock_t sequential_start_time = clock();  // Using clock_t to measure time
        
        printf("Sequential Execution\n");

        for (int i = 0; i < VECTOR_SIZE; i++) {
            result[i] = vector[i] + SCALAR;
        }

        clock_t sequential_end_time = clock();

        double sequential_time = (double)(sequential_end_time - sequential_start_time) / CLOCKS_PER_SEC;

        printf("Scalar product (sequential): %lld\n", sequential_product);
        printf("Time taken (sequential): %f seconds\n", sequential_time);

        /****************************************************************************************/

        /****************************************************************************************/
        printf("Parallel Execution\n");
        
        // i. STATIC schedule

        printf("\ni. STATIC schedule:\n");
        int static_chunk_sizes[] = {1, 5, 10, 20, 50};
        for (int i = 0; i < sizeof(static_chunk_sizes) / sizeof(int); i++) {
            static_execution_time = measure_time(result, vector, SCALAR, VECTOR_SIZE, static_chunk_sizes[i], "static");
            printf("Chunk size %d: %.6f seconds\n", static_chunk_sizes[i], static_execution_time);
        }

        // ii. DYNAMIC schedule
        printf("\nii. DYNAMIC schedule:\n");
        int dynamic_chunk_sizes[] = {1, 5, 10, 20, 50};
        for (int i = 0; i < sizeof(dynamic_chunk_sizes) / sizeof(int); i++) {
            dynamic_execution_time = measure_time(result, vector, SCALAR, VECTOR_SIZE, dynamic_chunk_sizes[i], "dynamic");
            printf("Chunk size %d: %.6f seconds\n", dynamic_chunk_sizes[i], dynamic_execution_time);
        }

        // iii. Demonstrating nowait clause
        printf("\niii. Demonstrating nowait clause:\n");
        double start_time = omp_get_wtime();
        #pragma omp parallel
        {
            #pragma omp for schedule(static) nowait
            for (int i = 0; i < VECTOR_SIZE / 2; i++) {
                result[i] = vector[i] + SCALAR;
            }

            #pragma omp for schedule(static) nowait
            for (int i = VECTOR_SIZE / 2; i < VECTOR_SIZE; i++) {
                result[i] = vector[i] + SCALAR;
            }
        }
        double end_time = omp_get_wtime();
        nowait_execution_time = end_time - start_time;
        printf("Execution time with nowait: %.6f seconds\n", nowait_execution_time);

        // printf("\nResult vector:\n");
        // print_vector(result, VECTOR_SIZE);

        /****************************************************************************************/

        double static_speedup = sequential_time / static_execution_time;  // double for accurate speedup calculation
        double dynamic_speedup = sequential_time / dynamic_execution_time;  // double for accurate speedup calculation
        double nowait_speedup = sequential_time / nowait_execution_time;  // double for accurate speedup calculation
        printf("Speed Up: %f\n", static_speedup);
        printf("Speed Up: %f\n", dynamic_speedup);
        printf("Speed Up: %f\n", nowait_speedup);

        free(vector);
        free(result);

        return 0;
    }








