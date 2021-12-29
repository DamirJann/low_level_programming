#include "./../include/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

const char *error_log_pattern() {
    static char s[300000] = {0};
    time_t t = time(NULL);
    strftime(s, sizeof(s) - 1, "%a %b %d %T %Z %Y    error   ---   ", localtime(&t));
    return s;

}


const char *info_log_pattern() {
    static char s[30000] = {0};
    time_t t = time(NULL);
    strftime(s, sizeof(s) - 1, "%a %b %d %T %Z %Y    info    ---   ", localtime(&t));
    return s;
}

FILE *open_file(char *path_to_input, char *modes) {
    FILE *input = fopen(path_to_input, modes);
    if (input == NULL) {
        fprintf(stderr, "%s can't open file (\"%s\")\n", error_log_pattern(), path_to_input);
        exit(EXIT_FAILURE);
    } else {
        fprintf(stdout, "%s successfully opened file (\"%s\")\n", info_log_pattern(), path_to_input);
    }
    return input;
}





