#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

pid_t child_pid; 


void sigint_handler(int signum) {
    printf("Child process will be interrupted by Ctrl+C\n");
    kill(child_pid, SIGINT); // Send SIGINT to the child process
}


void sigquit_handler(int signum) {
    printf("Received the quit signal and terminating parent process\n");
    exit(EXIT_SUCCESS);
}


void sigtstp_handler(int signum) {
    printf("Child process will suspended by Ctrl+Z\n");
    kill(child_pid, SIGTSTP); 
}

int main(int argc, char **argv) {
    pid_t pid;
    int status;

    if (argc < 2) {
        printf("Usage: %s <command> [args]\n", argv[0]);
        exit(-1);
    }

    
    signal(SIGINT, sigint_handler); 
    signal(SIGQUIT, sigquit_handler); 
    signal(SIGTSTP, sigtstp_handler); 

    pid = fork();
    if (pid == 0) { 
        execvp(argv[1], &argv[1]);
        printf("If you see this statement then execvp failed ;-(\n");
        perror("execvp");
        exit(-1);
    } else if (pid > 0) { 
        child_pid = pid; 
        printf("Wait for the child process to terminate\n");
        wait(&status); 
        if (WIFEXITED(status)) { 
            printf("Child process exited with status = %d\n", WEXITSTATUS(status));
        } else { 
            printf("Child process did not terminate normally!\n");
            
               
        }
        
        while(1) {
            pause();
        }
    } else { 
        perror("fork"); 
        exit(EXIT_FAILURE);
    }
    
    printf("[%ld]: Exiting program .....\n", (long)getpid());

    return 0;
}
