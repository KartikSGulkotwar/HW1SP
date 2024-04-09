#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define MAX_COMMAND_LENGTH 100
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

void* executeJob(void* arg) {
    while (1) {
        // Acquire lock to access the shared queue
        pthread_mutex_lock(&scheduler.mutex);

        // Check if there are pending jobs
        if (scheduler.job_count > 0) {
            // Get the job from the queue
            Job job = scheduler.jobs[--scheduler.job_count];

            // Release lock
            pthread_mutex_unlock(&scheduler.mutex);

            // Execute the job (for example, using system() or fork/exec)
            system(job.command);

            // Update job status
            // Here, you would update job.status, start_time, end_time, etc.

        } else {
            // Release lock
            pthread_mutex_unlock(&scheduler.mutex);

            // If no jobs, sleep for some time before checking again
            sleep(1);
        }
    }
    return NULL;
}

void submitJob(const char* command) {
    // Acquire lock to access the shared queue
    pthread_mutex_lock(&scheduler.mutex);

    // Add the job to the queue
    Job job;
    job.job_id = scheduler.job_count + 1;
    snprintf(job.command, sizeof(job.command), "%s", command);
    snprintf(job.status, sizeof(job.status), "Pending");
    // Set start_time, end_time, etc. as needed

    scheduler.jobs[scheduler.job_count++] = job;

    // Release lock
    pthread_mutex_unlock(&scheduler.mutex);
}

int main() {
    // Initialize mutex
    pthread_mutex_init(&scheduler.mutex, NULL);

    // Initialize job count
    scheduler.job_count = 0;

    // Create worker threads for job execution
    pthread_t worker;
    pthread_create(&worker, NULL, executeJob, NULL);

    // Submit some sample jobs
    submitJob("echo Job 1");
    submitJob("echo Job 2");

    // Wait for the worker thread to finish (in practice, you would have some termination condition)
    pthread_join(worker, NULL);

    // Destroy mutex
    pthread_mutex_destroy(&scheduler.mutex);

    return 0;
}
