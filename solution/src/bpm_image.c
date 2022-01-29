#include "../include/bpm_image.h"

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

enum read_status from_bmp(FILE *in, struct image *img) {
    struct bmp_header header = {0};
    if (fread(&header, sizeof(struct bmp_header), 1, in) != 1) {
        return READ_INVALID_HEADER;
    }

    if (header.bfType != B_F_TYPE){
        return READ_INVALID_SIGNATURE;
    }

    init_image(img, header.biWidth, header.biHeight);
    int padding = calculate_padding(img->width);

    for (uint64_t i = 0; i < img->height; i++) {
        if (fread(img->data + (img->width * i), sizeof(struct pixel), img->width, in) != img->width) {
            return READ_INVALID_BITS;
        }
        fseek(in, padding, SEEK_CUR);
    }

    return READ_OK;
}

enum write_status to_bmp(FILE *out, struct image const *img) {
    struct bmp_header header = make_bmp_header(*img);

    if (fwrite(&header, sizeof(struct bmp_header), 1, out) != 1) {
        return WRITE_ERROR;
    }

    int padding = calculate_padding(img->width);

    for (uint64_t i = 0; i < img->height; i++) {
        if (fwrite(img->data + (i * img->width), sizeof(struct pixel) , img->width, out) != img->width){
            return WRITE_ERROR;
        }

        if (fseek(out, padding, SEEK_CUR) != 0){
            return WRITE_ERROR;
        }
    }
    return WRITE_OK;
}

struct bmp_header make_bmp_header(struct image image) {
    int padding = calculate_padding(image.height);
    uint64_t pixel_image_count = image.width * image.height;

    return (struct bmp_header) {
            .biWidth = image.width,
            .biHeight = image.height,
            .bOffBits = sizeof(struct bmp_header),
            .bfReserved = B_F_RESERVED,
            .bfType = B_F_TYPE,
            .bfileSize = pixel_image_count * sizeof(struct pixel)
                         + padding * image.height + sizeof(struct bmp_header),
            .biBitCount = sizeof(struct pixel) * 8,
            .biClrImportant = BI_CLR_IMPORTANT,
            .biClrUsed = BI_CLR_USED,
            .biCompression = BI_COMPRESSION,
            .biPlanes = BI_PLANES,
            .biSize = BI_SIZE,
            .biSizeImage = pixel_image_count * sizeof(struct pixel)
                           + padding * image.height,
            .biXPelsPerMeter = BI_X_PELS_PER_METER,
            .biYPelsPerMeter = BI_Y_PELS_PER_METER,
    };
}

int static inline calculate_padding(uint64_t width) {
    return 4 - (int) (width * sizeof(struct pixel)) % 4;
}


