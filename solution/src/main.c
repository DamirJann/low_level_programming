#include <stdio.h>
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int MaxPathLength = 50;

void parse_args(int argc, char ** argv, char* path_to_input, char *path_to_output){
    if (argc != 2){
        fprintf(stderr, "Not enough parameter: got: %d, expected: 2", argc);
        exit(EXIT_FAILURE);
    }
    strcpy(path_to_input, )
}

int main( int argc, char** argv ) {
    (void) argc; (void) argv; // supress 'unused parameters' warning
    char *path_to_input = malloc(sizeof(char) * MaxPathLength);
    char *path_to_output = malloc(sizeof(char) * MaxPathLength);

    parse_args(argc, argv, path_to_input, path_to_output);

    free(path_to_input);
    free(path_to_output);
    return 0;
}
