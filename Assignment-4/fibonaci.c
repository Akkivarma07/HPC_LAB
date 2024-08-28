#include <omp.h>
#include <stdio.h>
#include <stdint.h>

long long int parallel_fib(int n) {
    if (n < 2) return n;

    long long int a = 0, b = 1, c;
    
    #pragma omp parallel
    {
        long long int local_a = a, local_b = b, local_c;
        
        #pragma omp for
        for (int i = 2; i <= n; i++) {
            local_c = local_a + local_b;
            local_a = local_b;
            local_b = local_c;
        }
        
        #pragma omp single
        {
            a = local_a;
            b = local_b;
        }
    }
    
    return b;
}

long long int sequential_fib(int n) {
    if (n < 2) return n;

    long long int a = 0, b = 1, c;
    for (int i = 2; i <= n; i++) {
        c = a + b;
        a = b;
        b = c;
    }
    return b;
}

int main() {
    int n;  // Number to compute Fibonacci number up to
    long long int result;  // Changed to long long int to handle larger values

    printf("Enter a number: ");
    scanf("%d", &n);

    /********************************************************/
    printf("Sequential Execution\n");

    double sequential_start_time = omp_get_wtime();  // Using omp_get_wtime() for high-precision timing

    result = sequential_fib(n);  // Call sequential version

    double sequential_end_time = omp_get_wtime(); 
    double sequential_time = sequential_end_time - sequential_start_time;  // double for precise time calculation
    printf("Fibonacci number F(%d) = %lld\n", n, result);
    printf("Time taken (sequential): %f seconds\n", sequential_time);

    /********************************************************/
    
    /********************************************************/

    printf("Parallel Execution\n");
    double parallel_start_time = omp_get_wtime();
    result = parallel_fib(n);
    double parallel_end_time = omp_get_wtime();
    double parallel_time = parallel_end_time - parallel_start_time;
    printf("Fibonacci number F(%d) = %lld\n", n, result);
    printf("Time taken (parallel): %f seconds\n", parallel_time);
    
    /********************************************************/

    double speedup = sequential_time / parallel_time;
    printf("Speed Up: %f\n", speedup);

    return 0;
}
