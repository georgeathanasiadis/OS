#ifndef TEXTFILE_H
#define TEXTFILE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure to store the text file content
typedef struct {
    char **lines;  // Array of lines
    int line_count; // Number of lines in the file
} TextFile;

// Function prototypes
TextFile *load_text_file(const char *filename);
char *get_random_line(TextFile *text_file);
void free_text_file(TextFile *text_file);

#endif
