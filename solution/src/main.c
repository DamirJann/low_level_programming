#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "./../include/utils.h"
#include "./../include/bpm_image.h"
#include "../include/image_transformation.h"


void parse_args(int argc, char **argv, char **path_to_input, char **path_to_output) {
    if (argc - 1 < 2) {
        fprintf(stderr, "Not enough parameters: got: %d, expected: 2\n", argc - 1);
        exit(EXIT_FAILURE);
    }
    if (argc - 1 > 2) {
        fprintf(stderr, "Too many parameters: got: %d, expected: 2\n", argc - 1);
        exit(EXIT_FAILURE);
    }
    *path_to_input = argv[1];
    *path_to_output = argv[2];
    fprintf(stdout, "%s parameters parsed (input = %s, output = %s)\n", info_log_pattern(), *path_to_input, *path_to_output);
}

int main(int argc, char **argv) {
    (void) argc;
    (void) argv; // supress 'unused parameters' warning
    char *path_to_input;
    char *path_to_output;

    parse_args(argc, argv, &path_to_input, &path_to_output);

    FILE* input = fopen(path_to_input, "rb");
    if  (input == NULL){
        fprintf(stderr, "%s can't open file (\"%s\")\n", error_log_pattern(), path_to_input);
        exit(EXIT_FAILURE);
    } else{
        fprintf(stdout, "%s successfully opened file (\"%s\")\n", info_log_pattern(), path_to_input);
    }

    struct image image;
    if (from_bmp(input, &image) != READ_OK) {
        fprintf(stderr, "%s can't read input file\n", error_log_pattern());
        exit(EXIT_FAILURE);
    } else {
        fprintf(stdout, "%s input file read (width = \"%lu\", height = \"%lu\")\n", info_log_pattern(), image.width, image.height);
    }

    FILE* output = fopen(path_to_output, "wb");
//    struct image rotated_image = rotate(image);
    to_bmp(output, &image);

    return 0;
}

