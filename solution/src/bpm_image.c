#include "../include/bpm_image.h"

enum read_status from_bmp(FILE *in, struct image *img) {
    struct bmp_header header = {0};
    if (fread(&header, sizeof(struct bmp_header), 1, in) != 1) {
        return READ_INVALID_HEADER;
    }

    if (header.bfType != B_F_TYPE){
        return READ_INVALID_SIGNATURE;
    }

    init_image(img, header.biWidth, header.biHeight);
    int padding = calculate_padding(img->width * sizeof(struct pixel));

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

    int padding = calculate_padding(img->width * sizeof(struct pixel));

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
    int padding = calculate_padding(image.height * sizeof(struct pixel));
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

int calculate_padding(uint64_t bytes) {
    return 4 - (int) bytes % 4;
}


