#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//represents a command from the file
typedef struct {
    int timestamp;       //time at which the command is executed
    char process_label[10]; //process label (e.g., "C1")
    char command;        //command type: 'S' for SPAWN, 'T' for TERMINATE
} Command;

//represent list of commands
typedef struct {
    Command *commands;  // Array of commands
    int size;           // Number of commands
    int capacity;       // Capacity of the array
} CommandList;

//function prototypes
CommandList *parse_command_file(const char *filename);
void free_command_list(CommandList *cmd_list);
Command *get_next_command(CommandList *cmd_list, int current_time);

#endif
