#include "process.h"
#include <unistd.h>
#include <string.h>

//spawn new process
pid_t spawn_process(void (*child_function)(void *), void *arg) {
    pid_t pid = fork();
    if (pid < 0) {
        handle_error("Failed to fork a new process");
    } else if (pid == 0) {
        // Child process: Call the provided function
        if (child_function != NULL) {
            child_function(arg);
        }
        exit(EXIT_SUCCESS);  // Ensure child exits after executing its task
    }
    return pid; // Parent process gets the PID of the child
}

//terminate a process
void terminate_process(pid_t pid) {
    if (kill(pid, SIGTERM) == -1) {
        handle_error("Failed to terminate process");
    }
}

//wait for a process to terminate
void wait_for_process(pid_t pid) {
    int status;
    if (waitpid(pid, &status, 0) == -1) {
        handle_error("Failed to wait for process");
    }

    if (WIFEXITED(status)) {
        printf("Process %d exited with status %d\n", pid, WEXITSTATUS(status));
    } else if (WIFSIGNALED(status)) {
        printf("Process %d was terminated by signal %d\n", pid, WTERMSIG(status));
    }
}

//error handling
void handle_error(const char *message) {
    perror(message);
    exit(EXIT_FAILURE);
}
