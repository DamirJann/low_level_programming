#ifndef GITHUB_ABSTRACT_IMAGE_H
#define GITHUB_ABSTRACT_IMAGE_H

#include <bits/stdint-uintn.h>
#include <bits/types/FILE.h>
#include <malloc.h>

struct __attribute__ ((packed)) pixel {
    uint8_t b, g, r;
};

struct point {
    uint64_t x;
    uint64_t y;
};

struct image {
    uint64_t width, height;
    struct pixel *data;
};

void init_image(struct image *img, uint64_t width, uint64_t height);

void destroy_image(struct image image);

static struct pixel inline image_get_pixel(struct image image, struct point point) {
    return image.data[image.width * point.y + point.x];
}

static void inline image_set_pixel(struct image image, struct point point, struct pixel pixel) {
    image.data[image.width * point.y + point.x] = pixel;
}

#endif //GITHUB_ABSTRACT_IMAGE_H
