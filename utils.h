#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//logging macros
#define LOG_INFO(msg, ...) log_message("INFO", msg, ##__VA_ARGS__)
#define LOG_ERROR(msg, ...) log_message("ERROR", msg, ##__VA_ARGS__)

//func prototypes
void log_message(const char *level, const char *format, ...);
void sleep_ms(int milliseconds);

#endif
