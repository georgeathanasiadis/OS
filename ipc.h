#ifndef IPC_H
#define IPC_H

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>

// Constants for permissions
#define IPC_PERMISSIONS 0666

// Semaphore functions
int create_semaphore(int num_semaphores);
void set_semaphore_value(int sem_id, int sem_num, int value);
void lock_semaphore(int sem_id, int sem_num);
void unlock_semaphore(int sem_id, int sem_num);
void destroy_semaphore(int sem_id);

// Shared memory functions
int create_shared_memory(size_t size);
void *attach_shared_memory(int shm_id);
void detach_shared_memory(void *shm_addr);
void destroy_shared_memory(int shm_id);

// Utility function for error handling
void handle_error(const char *message);

#endif
