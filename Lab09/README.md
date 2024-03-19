This C program demonstrating basic process management functionalities using signals in Unix-like operating systems. It spawns a child process to execute a given command and provides signal handlers for SIGINT, SIGQUIT, and SIGTSTP signals. The parent process waits for the child to terminate and handles signals accordingly.

## Functionalities:
Signal Handling:

SIGINT (Ctrl+C): Interrupts the child process.
SIGQUIT (Ctrl+\): Terminates the parent process.
SIGTSTP (Ctrl+Z): Suspends the child process.
Child Process Execution:

## Compilation:
gcc -o lab09 lab09.c
Link For Repository: https://github.com/KartikSGulkotwar/HWSP/tree/main/Lab09


