#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <ctype.h> 

#define MAX_COMMAND_LENGTH 100
#define MAX_ARGUMENTS 10
#define MAX_JOBS 100

typedef struct {
    int job_id;
    char command[MAX_COMMAND_LENGTH];
    char status[20];
    time_t start_time;
    time_t end_time;
} Job;

typedef struct {
    Job jobs[MAX_JOBS];
    int job_count;
    pthread_mutex_t mutex;
} JobScheduler;

JobScheduler scheduler;

void initializeScheduler() {
    scheduler.job_count = 0;
    pthread_mutex_init(&scheduler.mutex, NULL);
}

void* executeJob(void* arg) {
    
    return NULL;
}

void submitJob(char *command) {
    pthread_mutex_lock(&scheduler.mutex);

    if (scheduler.job_count >= MAX_JOBS) {
        printf("Maximum jobs limit reached.\n");
        pthread_mutex_unlock(&scheduler.mutex);
        return;
    }

    Job job;
    job.job_id = ++scheduler.job_count;
    strncpy(job.command, command, MAX_COMMAND_LENGTH - 1);
    job.command[MAX_COMMAND_LENGTH - 1] = '\0';
    strncpy(job.status, "Running", sizeof(job.status));
    job.status[sizeof(job.status) - 1] = '\0';
    job.start_time = time(NULL);

    scheduler.jobs[scheduler.job_count - 1] = job;
    pthread_mutex_unlock(&scheduler.mutex);

    printf("Job %d added to the queue\n", job.job_id);
}

void showJobs() {
    pthread_mutex_lock(&scheduler.mutex);

    if (scheduler.job_count == 0) {
        printf("No jobs in the queue\n");
    } else {
        printf("jobid command status\n");
        for (int i = 0; i < scheduler.job_count; ++i) {
            printf("%d %s %s\n", scheduler.jobs[i].job_id, scheduler.jobs[i].command, scheduler.jobs[i].status);
        }
    }

    pthread_mutex_unlock(&scheduler.mutex);
}

void submitHistory() {
    pthread_mutex_lock(&scheduler.mutex);

    if (scheduler.job_count == 0) {
        printf("No job history available\n");
    } else {
        printf("jobid command starttime endtime status\n");
        for (int i = 0; i < scheduler.job_count; ++i) {
            printf("%d %s %s %s %s\n", scheduler.jobs[i].job_id, scheduler.jobs[i].command, 
                   asctime(localtime(&scheduler.jobs[i].start_time)), asctime(localtime(&scheduler.jobs[i].end_time)),
                   scheduler.jobs[i].status);
        }
    }

    pthread_mutex_unlock(&scheduler.mutex);
}

int main() {
    initializeScheduler();

    pthread_t worker;
    pthread_create(&worker, NULL, executeJob, NULL);

    char input[MAX_COMMAND_LENGTH];
    while (1) {
        printf("Enter command> ");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0';

        char *command = strtok(input, " ");
        if (command == NULL) {
            printf("Invalid command\n");
            continue;
        }

        
        for (int i = 0; command[i]; ++i) {
            command[i] = tolower(command[i]);
        }

        if (strcmp(command, "submit") == 0) {
            char *job_command = strtok(NULL, "");
            if (job_command != NULL) {
                submitJob(job_command);
            } else {
                printf("Invalid command format. Usage: submit <job_command>\n");
            }
        } else if (strcmp(command, "showjobs") == 0 || strcmp(command, "jobs") == 0) {
            showJobs();
        } else if (strcmp(command, "submithistory") == 0) {
            submitHistory();
        } else {
            printf("Invalid command\n");
        }
    }

    pthread_join(worker, NULL);
    pthread_mutex_destroy(&scheduler.mutex);
    return 0;
}