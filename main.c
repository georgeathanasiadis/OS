#include "parser.h"
#include "textfile.h"
#include "ipc.h"
#include "process.h"
#include "utils.h"

#define MAX_PROCESSES 5
#define SHARED_MEM_SIZE 1024

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <command_file> <text_file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *command_file = argv[1];
    const char *text_file = argv[2];

    LOG_INFO("Loading command file: %s", command_file);
    CommandList *cmd_list = parse_command_file(command_file);

    LOG_INFO("Loading text file: %s", text_file);
    TextFile *txt_file = load_text_file(text_file);

    LOG_INFO("Setting up shared memory and semaphores");
    int shm_id = create_shared_memory(SHARED_MEM_SIZE);
    void *shm_addr = attach_shared_memory(shm_id);

    int sem_id = create_semaphore(MAX_PROCESSES);
    for (int i = 0; i < MAX_PROCESSES; i++) {
        set_semaphore_value(sem_id, i, 0);
    }

    LOG_INFO("Starting main event loop");
    ChildProcess child_processes[MAX_PROCESSES] = {0};
    int current_time = 0;

    while (1) {
        LOG_INFO("Current time: %d", current_time);

        //execute commands for current time
        Command *cmd = get_next_command(cmd_list, current_time);
        while (cmd != NULL) {
            if (cmd->command == 'S') { // SPAWN
                LOG_INFO("Spawning process %s", cmd->process_label);

                //find an available slot for new process
                int found = 0;
                for (int i = 0; i < MAX_PROCESSES; i++) {
                    if (!child_processes[i].active) {
                        child_processes[i].sem_id = sem_id;
                        child_processes[i].pid = spawn_process(child_main, shm_addr);
                        child_processes[i].active = 1;
                        found = 1;
                        break;
                    }
                }
                if (!found) {
                    LOG_ERROR("Maximum process limit reached. Cannot spawn %s.", cmd->process_label);
                }
            } else if (cmd->command == 'T') { // TERMINATE
                LOG_INFO("Terminating process %s", cmd->process_label);

                //find the process to terminate
                int found = 0;
                for (int i = 0; i < MAX_PROCESSES; i++) {
                    if (child_processes[i].active) {
                        terminate_process(child_processes[i].pid);
                        wait_for_process(child_processes[i].pid);
                        child_processes[i].active = 0;
                        found = 1;
                        break;
                    }
                }
                if (!found) {
                    LOG_ERROR("No active process found for termination: %s", cmd->process_label);
                }
            }
            cmd = get_next_command(cmd_list, current_time);
        }

        //randomly send messages to active processes
        for (int i = 0; i < MAX_PROCESSES; i++) {
            if (child_processes[i].active) {
                char *line = get_random_line(txt_file);
                snprintf((char *)shm_addr, SHARED_MEM_SIZE, "%s", line);
                unlock_semaphore(sem_id, i);
                break;
            }
        }

        current_time++;
        sleep_ms(1000); // 1 second per loop cycle
    }

    LOG_INFO("Cleaning up resources");
    for (int i = 0; i < MAX_PROCESSES; i++) {
        if (child_processes[i].active) {
            terminate_process(child_processes[i].pid);
            wait_for_process(child_processes[i].pid);
        }
    }

    destroy_semaphore(sem_id);
    detach_shared_memory(shm_addr);
    destroy_shared_memory(shm_id);
    free_command_list(cmd_list);
    free_text_file(txt_file);

    LOG_INFO("Program terminated successfully");
    return EXIT_SUCCESS;
}
