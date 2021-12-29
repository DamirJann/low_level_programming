#ifndef GITHUB_ABSTRACT_IMAGE_H
#define GITHUB_ABSTRACT_IMAGE_H
#include <bits/stdint-uintn.h>
#include <bits/types/FILE.h>
#include <malloc.h>

struct pixel {
    uint8_t b, g, r;
};

static struct pixel ZERO_PIXEL = (struct pixel) {.b = 0, .g = 0, .r = 0};

struct image {
    uint64_t width, height;
    struct pixel *data;
};

void init_image(struct image * img, uint64_t width, uint64_t height);

void destroy_image(struct image image);

struct pixel get_pixel(struct image image, uint64_t height, uint64_t width);

void set_pixel(struct image image, uint64_t height, uint64_t width, struct pixel pixel);


#endif //GITHUB_ABSTRACT_IMAGE_H
