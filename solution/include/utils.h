#ifndef GITHUB_UTILS_H
#define GITHUB_UTILS_H

#include <bits/types/FILE.h>
#include <stdarg.h>
#include <stdio.h>

static char *error = "error";
static char *info = "info";

const char* info_log_pattern();
const char* error_log_pattern();

FILE* open_file(char* path_to_input, char* modes);


#endif //GITHUB_UTILS_H
