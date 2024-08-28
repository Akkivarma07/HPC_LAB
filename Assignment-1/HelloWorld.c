#include <stdio.h>
#include <omp.h>

int main(){
    int num_threads;

    printf("Enter number of threads: ");
    scanf("%d", &num_threads);

    omp_set_num_threads(num_threads);

    printf("Sequential Execution: \n");

    for(int i=0; i < num_threads; i++){
        printf("Hello, World from thread %d\n", i);
    }

    printf("Parallel Execution: \n");
    #pragma omp parallel
    {
        printf("Hello, World from thread %d\n", omp_get_thread_num());
    }

    return  0;
}