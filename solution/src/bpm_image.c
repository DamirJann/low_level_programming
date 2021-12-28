#include "../include/bpm_image.h"
#include "../include/abstract_image.h"


enum read_status from_bmp(FILE *in, struct image *img) {
    struct bmp_header header = {0};
    if (fread(&header, sizeof(struct bmp_header), 1, in) != 1) {
        return READ_INVALID_HEADER;
    }

    init_image(img, header.biHeight, header.biWidth);
    int padding = calculate_byte_padding(img->width * sizeof(struct pixel));

    for (uint64_t i = 0; i < img->height; i++){
        fseek(in, padding , SEEK_CUR);
        if (fread(img->data + (img->width * i), sizeof(struct pixel) , img->width, in) != img->width) {
            return READ_INVALID_BITS;
        }
    }



    return READ_OK;
}

enum write_status to_bmp(FILE *out, struct image const *img) {
    struct bmp_header header = (struct bmp_header){
        .biHeight = img->height,
        .biWidth = img->width
    };

    if (fwrite(&header, sizeof(struct bmp_header), 1, out) != 1){
        return WRITE_ERROR;
    }

    fprintf(stderr,"%lu\n", sizeof(header));
    int padding = calculate_byte_padding(img->width * sizeof(struct pixel));

    for (uint64_t i = 0; i < img->height; i++){
        fwrite(img->data + (i * img->width), sizeof(struct pixel) * img->width, 1, out);
        fwrite("000", padding, 1, out);
    }
    return WRITE_OK;
}


int calculate_byte_padding(uint64_t width){
    return 4 - (int) width % 4;
}


