#include "../include/abstract_image.h"

typedef struct image image;

void init_image(struct image *img, uint64_t width, uint64_t height) {
    img->width = width;
    img->height = height;
    img->data = malloc((width * height) * sizeof(struct pixel));

    for (uint64_t i = 0; i < width * height; i++) {
        img->data[i] = ZERO_PIXEL;
    }
}

struct pixel get_pixel(const struct image image, uint64_t height, uint64_t width) {
    return image.data[image.width * height + width];
}

void set_pixel(struct image image, uint64_t height, uint64_t width, struct pixel pixel) {
    image.data[image.width * height + width] = pixel;
}

void destroy_image(struct image image) {
    free(image.data);
}
