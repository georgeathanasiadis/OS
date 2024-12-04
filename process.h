#ifndef PROCESS_H
#define PROCESS_H

#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>

//structure to track child process information
typedef struct {
    pid_t pid;      //process ID
    int sem_id;     //semaphore ID for the process
    int active;     //status: 1 if active, 0 if not
} ChildProcess;

//function prototypes
pid_t spawn_process(void (*child_function)(void *), void *arg);
void terminate_process(pid_t pid);
void wait_for_process(pid_t pid);
void handle_error(const char *message);

#endif
