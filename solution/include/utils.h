#ifndef GITHUB_UTILS_H
#define GITHUB_UTILS_H

#include <stdarg.h>
#include <stdio.h>

const char* info_log_pattern();
const char* error_log_pattern();

FILE* open_file(char* path, char* modes);

enum arg_parsing_status{
    OK,
    ERROR
};

struct arg_parsing_result error_arg_parsing_res();

struct arg_parsing_result{
    enum arg_parsing_status status;
    char *path_to_input;
    char *path_to_output;
};

#endif //GITHUB_UTILS_H
