#ifndef GITHUB_BPM_IMAGE_H
#define GITHUB_BPM_IMAGE_H

#include "abstract_image.h"

/*  serializer   */
enum write_status {
    WRITE_OK = 0,
    WRITE_ERROR
    /* коды других ошибок  */
};


/*  deserializer   */
enum read_status {
    READ_OK = 0,
    READ_INVALID_SIGNATURE,
    READ_INVALID_BITS,
    READ_INVALID_HEADER
    /* коды других ошибок  */
};

enum read_status from_bmp(FILE *in, struct image *img);

enum write_status to_bmp(FILE *out, struct image const *img);

struct bmp_header make_bmp_header(struct image image);

int static inline calculate_padding(uint64_t width);

#endif //GITHUB_BPM_IMAGE_H
