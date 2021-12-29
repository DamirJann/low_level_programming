#include <stdbool.h>
#include <time.h>
#include <stdlib.h>
#include "./../include/utils.h"


static char * error = "error";
static char * info = "info";

const char* error_log_pattern(){
    static char s[30];
    time_t t = time(NULL);
    size_t count = strftime(s , sizeof(s) - 1, "%a %b %d %T %Z %Y", localtime(&t));
    sprintf(s + count, "  %s   ---", error);
    return s;
}


const char* info_log_pattern(){
    static char s[30];
    time_t t = time(NULL);
    size_t count = strftime(s , sizeof(s) - 1, "%a %b %d %T %Z %Y", localtime(&t));
    sprintf(s + count, "   %s   ---", info);
    return s;
}

FILE* open_file(char* path_to_input, char* modes){
    FILE* input = fopen(path_to_input, modes);
    if  (input == NULL){
        fprintf(stderr, "%s can't open file (\"%s\")\n", error_log_pattern(), path_to_input);
        exit(EXIT_FAILURE);
    } else{
        fprintf(stdout, "%s successfully opened file (\"%s\")\n", info_log_pattern(), path_to_input);
    }
    return input;
}





