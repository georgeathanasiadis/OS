#include "utils.h"
#include <stdarg.h>
#include <unistd.h>

//log messages w/ timestamp & level
void log_message(const char *level, const char *format, ...) {
    va_list args;
    va_start(args, format);

    //print timestamp
    time_t now = time(NULL);
    struct tm *local_time = localtime(&now);
    printf("[%02d:%02d:%02d] [%s] ", local_time->tm_hour, local_time->tm_min, local_time->tm_sec, level);

    //print custom message
    vprintf(format, args);
    printf("\n");

    va_end(args);
}

//sleep for a given number of milliseconds
void sleep_ms(int milliseconds) {
    usleep(milliseconds * 1000); // Convert ms to µs
}
