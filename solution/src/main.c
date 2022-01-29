#include "../include/image_transformation.h"
#include "./../include/bpm_image.h"
#include "./../include/utils.h"
#include <stdio.h>
#include <stdlib.h>

struct arg_parsing_result parse_args(int argc, char **argv) {
    if (argc - 1 < 2) {
        fprintf(stderr, "%s not enough parameters: got: %d, expected: 2\n", error_log_pattern(), argc - 1);
        return error_arg_parsing_res();
    }
    if (argc - 1 > 2) {
        fprintf(stderr, "%s too many parameters: got: %d, expected: 2\n", error_log_pattern(), argc - 1);
        return error_arg_parsing_res();
    }
    struct arg_parsing_result res = (struct arg_parsing_result) {.status = OK};
    res.path_to_input = argv[1];
    res.path_to_output = argv[2];
    fprintf(stdout, "%s parameters parsed (input = \"%s\", output = \"%s\")\n", info_log_pattern(), res.path_to_input,
            res.path_to_output);
    return res;
}

int main(int argc, char **argv) {
    struct arg_parsing_result res = parse_args(argc, argv);
    if (res.status == ERROR) {
        return -1;
    }

    FILE *input = open_file(res.path_to_input, "rb");

    struct image image;
    if (from_bmp(input, &image) != READ_OK) {
        fprintf(stderr, "%s can't read input file\n", error_log_pattern());
        return -1;
    } else {
        fprintf(stdout, "%s input file read (width = %lu, height = %lu)\n", info_log_pattern(), image.width,
                image.height);
        fclose(input);
    }

    struct image rotated_image = rotate(image);
    destroy_image(image);

    FILE *output = open_file(res.path_to_output, "wb");
    if (to_bmp(output, &rotated_image) != WRITE_OK) {
        fprintf(stderr, "%s can't write to output file\n", error_log_pattern());
        return -1;
    } else {
        fprintf(stdout, "%s output file was written (width = %lu, height = %lu)\n", info_log_pattern(),
                rotated_image.width, rotated_image.height);
    }

    destroy_image(rotated_image);
    fclose(output);
    return 0;
}
