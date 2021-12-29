#include "../include/image_transformation.h"
#include "./../include/bpm_image.h"
#include "./../include/utils.h"
#include <stdio.h>
#include <stdlib.h>

void parse_args(int argc, char **argv, char **path_to_input, char **path_to_output) {
    if (argc - 1 < 2) {
        fprintf(stderr, "%s not enough parameters: got: %d, expected: 2\n", error_log_pattern(),argc - 1);
        exit(EXIT_FAILURE);
    }
    if (argc - 1 > 2) {
        fprintf(stderr, "%s too many parameters: got: %d, expected: 2\n", error_log_pattern(),argc - 1);
        exit(EXIT_FAILURE);
    }
    *path_to_input = argv[1];
    *path_to_output = argv[2];
    fprintf(stdout, "%s parameters parsed (input = \"%s\", output = \"%s\")\n", info_log_pattern(), *path_to_input, *path_to_output);
}

int main(int argc, char **argv) {
    char *path_to_input, *path_to_output;

    parse_args(argc, argv, &path_to_input, &path_to_output);

    FILE* input = open_file(path_to_input, "rb");
    FILE* output = open_file(path_to_output, "wb");

    struct image image;
    if (from_bmp(input, &image) != READ_OK) {
        fprintf(stderr, "%s can't read input file\n", error_log_pattern());
        exit(EXIT_FAILURE);
    } else {
        fprintf(stdout, "%s input file read (width = %lu, height = %lu)\n", info_log_pattern(), image.width, image.height);
    }

    struct image rotated_image = rotate(image);

    if (to_bmp(output, &rotated_image) != WRITE_OK){
        fprintf(stderr, "%s can't write to output file\n", error_log_pattern());
        exit(EXIT_FAILURE);
    } else {
        fprintf(stdout, "%s output file was written (width = %lu, height = %lu)\n", info_log_pattern(), rotated_image.width, rotated_image.height);
    }

    destroy_image(image);
    destroy_image(rotated_image);
    fclose(input);
    fclose(output);
    return 0;
}


