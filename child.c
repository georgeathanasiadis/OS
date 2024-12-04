#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <signal.h>
#include <time.h>

// Global variables for IPC
int semaphore_id;
void *shared_memory;
int running = 1;

// Function prototypes
void handle_termination(int signal);
void child_main(void *arg);

// Handle termination signals from the parent
void handle_termination(int signal) {
    running = 0; // Set the running flag to 0 to indicate termination
}

// Main child function
void child_main(void *arg) {
    int messages_received = 0;       // Counter for received messages
    int start_time = time(NULL);     // Record the start time

    // Attach to shared memory
    shared_memory = arg;

    // Install signal handler for SIGTERM
    struct sigaction action;
    memset(&action, 0, sizeof(action));
    action.sa_handler = handle_termination;
    sigaction(SIGTERM, &action, NULL);

    printf("[Child %d] Waiting for messages...\n", getpid());

    // Child process loop
    while (running) {
        // Simulate receiving a message (wait for semaphore lock)
        struct sembuf operation = {0, -1, 0}; // Wait operation
        if (semop(semaphore_id, &operation, 1) == -1) {
            perror("[Child] Semaphore wait failed");
            exit(EXIT_FAILURE);
        }

        if (!running) break; // Exit if the termination flag is set

        // Read the message from shared memory
        char *message = (char *)shared_memory;
        printf("[Child %d] Received message: %s\n", getpid(), message);
        messages_received++;

        // Simulate processing the message
        sleep(1); // Simulated processing time
    }

    // Calculate total runtime
    int end_time = time(NULL);
    int total_runtime = end_time - start_time;

    // Print final statistics
    printf("[Child %d] Terminating. Messages received: %d, Total runtime: %d seconds.\n",
           getpid(), messages_received, total_runtime);

    // Detach from shared memory
    if (shmdt(shared_memory) == -1) {
        perror("[Child] Failed to detach shared memory");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}
