#include "parser.h"

//initialize CommandList
static CommandList *initialize_command_list(int initial_capacity) {
    CommandList *cmd_list = (CommandList *)malloc(sizeof(CommandList));
    if (!cmd_list) {
        perror("Failed to allocate memory for CommandList");
        exit(EXIT_FAILURE);
    }

    cmd_list->commands = (Command *)malloc(initial_capacity * sizeof(Command));
    if (!cmd_list->commands) {
        perror("Failed to allocate memory for Command array");
        free(cmd_list);
        exit(EXIT_FAILURE);
    }

    cmd_list->size = 0;
    cmd_list->capacity = initial_capacity;
    return cmd_list;
}

//add command to the CommandList
static void add_command(CommandList *cmd_list, int timestamp, const char *process_label, char command) {
    if (cmd_list->size == cmd_list->capacity) {
        cmd_list->capacity *= 2;
        cmd_list->commands = (Command *)realloc(cmd_list->commands, cmd_list->capacity * sizeof(Command));
        if (!cmd_list->commands) {
            perror("Failed to reallocate memory for Command array");
            free(cmd_list);
            exit(EXIT_FAILURE);
        }
    }

    Command *new_command = &cmd_list->commands[cmd_list->size++];
    new_command->timestamp = timestamp;
    strncpy(new_command->process_label, process_label, sizeof(new_command->process_label) - 1);
    new_command->process_label[sizeof(new_command->process_label) - 1] = '\0'; // Ensure null-termination
    new_command->command = command;
}

//parse command file
CommandList *parse_command_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Failed to open command file");
        exit(EXIT_FAILURE);
    }

    CommandList *cmd_list = initialize_command_list(10); // Initial capacity of 10 commands
    char line[256];

    while (fgets(line, sizeof(line), file)) {
        int timestamp;
        char process_label[10];
        char command;

        if (sscanf(line, "%d %s %c", &timestamp, process_label, &command) != 3) {
            fprintf(stderr, "Invalid command format: %s", line);
            continue; // Skip invalid lines
        }

        add_command(cmd_list, timestamp, process_label, command);
    }

    fclose(file);
    return cmd_list;
}

//retrieve the next command based on current timestamp
Command *get_next_command(CommandList *cmd_list, int current_time) {
    for (int i = 0; i < cmd_list->size; i++) {
        if (cmd_list->commands[i].timestamp == current_time) {
            return &cmd_list->commands[i];
        }
    }
    return NULL; // No command for the current time
}

//free memory allocated for the CommandList
void free_command_list(CommandList *cmd_list) {
    if (cmd_list) {
        free(cmd_list->commands);
        free(cmd_list);
    }
}
