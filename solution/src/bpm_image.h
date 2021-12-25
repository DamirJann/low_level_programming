#include <bits/types/FILE.h>

#ifndef GITHUB_BPM_IMAGE_H
#define GITHUB_BPM_IMAGE_H

#endif //GITHUB_BPM_IMAGE_H

#include "abstract_image.h"

/*  deserializer   */
enum read_status  {
    READ_OK = 0,
    READ_INVALID_SIGNATURE,
    READ_INVALID_BITS,
    READ_INVALID_HEADER
    /* коды других ошибок  */
};

enum read_status from_bmp( FILE* in, struct image* img );

/*  serializer   */
enum  write_status  {
    WRITE_OK = 0,
    WRITE_ERROR
    /* коды других ошибок  */
};

enum write_status to_bmp( FILE* out, struct image const* img );

void read_bmp_by_path(char* path_to_file);

void write_bmp_by_path(char *path_to_file);