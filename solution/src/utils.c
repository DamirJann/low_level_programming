#include "./../include/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static char log_buffer[200] = {0};

const char *error_log_pattern() {
    time_t t = time(NULL);
    strftime(log_buffer, sizeof(log_buffer) - 1, "%a %b %d %T %Z %Y    error   ---   ", localtime(&t));
    return log_buffer;
}

const char *info_log_pattern() {
    time_t t = time(NULL);
    strftime(log_buffer, sizeof(log_buffer) - 1, "%a %b %d %T %Z %Y    info    ---   ", localtime(&t));
    return log_buffer;
}

FILE *open_file(char *path, char *modes) {
    FILE *input = fopen(path, modes);
    if (input == NULL) {
        fprintf(stderr, "%s can't open file (\"%s\")\n", error_log_pattern(), path);
        exit(EXIT_FAILURE);
    } else {
        fprintf(stdout, "%s successfully opened file (\"%s\")\n", info_log_pattern(), path);
    }
    return input;
}

struct arg_parsing_result error_arg_parsing_res(){
    return (struct arg_parsing_result){.status = ERROR};
}
