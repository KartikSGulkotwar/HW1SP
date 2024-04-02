#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

struct ThreadArgs {
    double *a;
    double sum;
    int N;
    int size;
    long tid;
};

void *compute(void *arg) {
    struct ThreadArgs *args = (struct ThreadArgs *)arg;
    int myStart, myEnd, myN, i;

    // Determine start and end of computation for the current thread
    myN = args->N / args->size;
    myStart = args->tid * myN;
    myEnd = myStart + myN;
    if (args->tid == (args->size - 1))
        myEnd = args->N;

    // Compute partial sum
    double mysum = 0.0;
    for (i = myStart; i < myEnd; i++)
        mysum += args->a[i];

    // Print partial sum for each thread
    printf("Partial sum for thread %ld: %g\n", args->tid, mysum);

    // Grab the lock, update global sum, and release lock
    pthread_mutex_lock(&mutex);
    args->sum += mysum;
    pthread_mutex_unlock(&mutex);

    return NULL;
}

int main(int argc, char **argv) {
    long i;
    pthread_t *tid;

    if (argc != 3) {
        printf("Usage: %s <# of elements> <# of threads>\n", argv[0]);
        exit(-1);
    }

    int N = atoi(argv[1]); // no. of elements
    int size = atoi(argv[2]); // no. of threads

    // Allocate vector and initialize
    double *a = (double *)malloc(sizeof(double) * N);
    for (i = 0; i < N; i++)
        a[i] = (double)(i + 1);

    // Create threads
    tid = (pthread_t *)malloc(sizeof(pthread_t) * size);
    struct ThreadArgs *threadArgs = (struct ThreadArgs *)malloc(sizeof(struct ThreadArgs) * size);
    for (i = 0; i < size; i++) {
        threadArgs[i].a = a;
        threadArgs[i].sum = 0.0;
        threadArgs[i].N = N;
        threadArgs[i].size = size;
        threadArgs[i].tid = i;
        pthread_create(&tid[i], NULL, compute, (void *)&threadArgs[i]);
    }

    // Wait for threads to complete
    for (i = 0; i < size; i++)
        pthread_join(tid[i], NULL);

    // Accumulate partial sums
    double total = 0.0;
    for (i = 0; i < size; i++)
        total += threadArgs[i].sum;

    printf("The total is %g, it should be equal to %g\n", total, ((double)N * (N + 1)) / 2);

    // Free allocated memory
    free(a);
    free(tid);
    free(threadArgs);

    return 0;
}
