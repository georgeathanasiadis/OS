#include "ipc.h"
#include <errno.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <unistd.h>

// Semaphore functions
int create_semaphore(int num_semaphores) {
    int sem_id = semget(IPC_PRIVATE, num_semaphores, IPC_CREAT | IPC_PERMISSIONS);
    if (sem_id == -1) {
        handle_error("Failed to create semaphore");
    }
    return sem_id;
}

void set_semaphore_value(int sem_id, int sem_num, int value) {
    if (semctl(sem_id, sem_num, SETVAL, value) == -1) {
        handle_error("Failed to set semaphore value");
    }
}

void lock_semaphore(int sem_id, int sem_num) {
    struct sembuf operation = {sem_num, -1, 0};
    if (semop(sem_id, &operation, 1) == -1) {
        handle_error("Failed to lock semaphore");
    }
}

void unlock_semaphore(int sem_id, int sem_num) {
    struct sembuf operation = {sem_num, 1, 0};
    if (semop(sem_id, &operation, 1) == -1) {
        handle_error("Failed to unlock semaphore");
    }
}

void destroy_semaphore(int sem_id) {
    if (semctl(sem_id, 0, IPC_RMID) == -1) {
        handle_error("Failed to destroy semaphore");
    }
}

// Shared memory functions
int create_shared_memory(size_t size) {
    int shm_id = shmget(IPC_PRIVATE, size, IPC_CREAT | IPC_PERMISSIONS);
    if (shm_id == -1) {
        handle_error("Failed to create shared memory");
    }
    return shm_id;
}

void *attach_shared_memory(int shm_id) {
    void *shm_addr = shmat(shm_id, NULL, 0);
    if (shm_addr == (void *)-1) {
        handle_error("Failed to attach shared memory");
    }
    return shm_addr;
}

void detach_shared_memory(void *shm_addr) {
    if (shmdt(shm_addr) == -1) {
        handle_error("Failed to detach shared memory");
    }
}

void destroy_shared_memory(int shm_id) {
    if (shmctl(shm_id, IPC_RMID, NULL) == -1) {
        handle_error("Failed to destroy shared memory");
    }
}

// Utility function for error handling
void handle_error(const char *message) {
    fprintf(stderr, "%s: %s\n", message, strerror(errno));
    exit(EXIT_FAILURE);
}
