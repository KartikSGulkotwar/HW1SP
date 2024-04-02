Program for  Multi-threaded Summation

This program computes the sum of elements in an array using multiple threads. Each thread computes a portion of the array and then aggregates the partial sums to get the total sum.

## Functionalities
The program takes two command-line arguments:
  The number of elements in the array.
   The number of threads to use for computation.
It creates an array of double precision floating-point numbers and initializes it with sequential values starting from 1.
 Threads are created to concurrently compute partial sums of the array.
 Each thread computes a portion of the array and adds its partial sum to a global sum.
 A mutex lock is used to ensure thread safety while updating the global sum.
 After all threads complete their computations, the program aggregates the partial sums to get the total sum.
 The program outputs the total sum and compares it with the expected sum ((N * (N + 1)) / 2, where N is the number of elements in the array).

## Compilation

gcc -o -Wall -pthread lab11.c -o lab11

Link for Repository: https://github.com/KartikSGulkotwar/HWSP/tree/main/lab11
