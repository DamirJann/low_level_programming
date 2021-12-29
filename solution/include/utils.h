#ifndef GITHUB_UTILS_H
#define GITHUB_UTILS_H

#include <stdio.h>
#include <stdarg.h>
#include <bits/types/FILE.h>

const char* info_log_pattern();
const char* error_log_pattern();

FILE* open_file(char*, char*);


#endif //GITHUB_UTILS_H
