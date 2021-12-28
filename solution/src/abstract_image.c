#include "../include/abstract_image.h"

void init_image(struct image *img, uint64_t width, uint64_t height) {
    img->width = width;
    img->height = height;
    img->data = malloc((width * height) * sizeof(struct pixel));

    for (uint64_t i = 0; i < width * height ; i++) {
        img->data[i] = (struct pixel) {.b = 0, .g = 0, .r = 0};
    }
}

void destroy_image(struct image image) {
    free(image.data);
}
