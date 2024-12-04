#include "textfile.h"

//load txt file into memory, line by line
TextFile *load_text_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Failed to open text file");
        exit(EXIT_FAILURE);
    }

    TextFile *text_file = (TextFile *)malloc(sizeof(TextFile));
    if (!text_file) {
        perror("Failed to allocate memory for TextFile");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    text_file->lines = NULL;
    text_file->line_count = 0;

    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    //read lines into the TextFile structure
    while ((read = getline(&line, &len, file)) != -1) {
        //remove newline character if present
        if (line[read - 1] == '\n') {
            line[read - 1] = '\0';
        }

        text_file->line_count++;
        text_file->lines = (char **)realloc(text_file->lines, text_file->line_count * sizeof(char *));
        if (!text_file->lines) {
            perror("Failed to reallocate memory for lines");
            free(line);
            fclose(file);
            free_text_file(text_file);
            exit(EXIT_FAILURE);
        }

        text_file->lines[text_file->line_count - 1] = strdup(line);
        if (!text_file->lines[text_file->line_count - 1]) {
            perror("Failed to duplicate line");
            free(line);
            fclose(file);
            free_text_file(text_file);
            exit(EXIT_FAILURE);
        }
    }

    free(line);
    fclose(file);
    return text_file;
}

// Get a random line from the loaded text file
char *get_random_line(TextFile *text_file) {
    if (text_file->line_count == 0) {
        fprintf(stderr, "TextFile contains no lines\n");
        return NULL;
    }
    int random_index = rand() % text_file->line_count;
    return text_file->lines[random_index];
}

// Free the memory allocated for the TextFile
void free_text_file(TextFile *text_file) {
    if (text_file) {
        for (int i = 0; i < text_file->line_count; i++) {
            free(text_file->lines[i]);
        }
        free(text_file->lines);
        free(text_file);
    }
}
