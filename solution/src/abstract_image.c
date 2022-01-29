#include "../include/abstract_image.h"

typedef struct image image;

void init_image(struct image *img, uint64_t width, uint64_t height) {
    img->width = width;
    img->height = height;
    img->data = calloc(width * height, sizeof(struct pixel));
}

void destroy_image(struct image image) {
    free(image.data);
}
