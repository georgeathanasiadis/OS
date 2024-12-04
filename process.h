#ifndef PROCESS_H
#define PROCESS_H

#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>

// Structure to track child process information
typedef struct {
    pid_t pid;      // Process ID
    int sem_id;     // Semaphore ID for the process
    int active;     // Status: 1 if active, 0 otherwise
} ChildProcess;

// Function prototypes
pid_t spawn_process(void (*child_function)(void *), void *arg);
void terminate_process(pid_t pid);
void wait_for_process(pid_t pid);
void handle_error(const char *message);

#endif
