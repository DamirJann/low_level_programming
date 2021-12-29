#ifndef GITHUB_BPM_IMAGE_H
#define GITHUB_BPM_IMAGE_H

#include "abstract_image.h"

struct __attribute__((packed)) bmp_header{
    uint16_t bfType;
    uint32_t bfileSize;
    uint32_t bfReserved;
    uint32_t bOffBits;
    uint32_t biSize;
    uint32_t biWidth;
    uint32_t biHeight;
    uint16_t biPlanes;
    uint16_t biBitCount;
    uint32_t biCompression;
    uint32_t biSizeImage;
    uint32_t biXPelsPerMeter;
    uint32_t biYPelsPerMeter;
    uint32_t biClrUsed;
    uint32_t biClrImportant;
};



/*  deserializer   */
enum read_status {
    READ_OK = 0,
    READ_INVALID_SIGNATURE,
    READ_INVALID_BITS,
    READ_INVALID_HEADER
    /* коды других ошибок  */
};

enum bmp_header_values{
    B_F_RESERVED = 0,
    B_F_TYPE = 19778,
    BI_CLR_IMPORTANT = 0,
    BI_CLR_USED = 0,
    BI_COMPRESSION = 0,
    BI_PLANES = 1,
    BI_X_PELS_PER_METER = 2834,
    BI_Y_PELS_PER_METER = 2834,
    BI_SIZE = 40
};

enum read_status from_bmp(FILE *in, struct image *img);

/*  serializer   */
enum write_status {
    WRITE_OK = 0,
    WRITE_ERROR
    /* коды других ошибок  */
};

enum write_status to_bmp(FILE *out, struct image const *img);

struct bmp_header make_bmp_header(struct image image);

int calculate_padding(uint64_t bytes);

#endif //GITHUB_BPM_IMAGE_H
