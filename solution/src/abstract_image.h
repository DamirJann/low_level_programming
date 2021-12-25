#include <stdint-gcc.h>

#ifndef GITHUB_ABSTRACT_IMAGE_H
#define GITHUB_ABSTRACT_IMAGE_H

#endif //GITHUB_ABSTRACT_IMAGE_H

struct pixel { uint8_t b, g, r; };

struct image {
    uint64_t width, height;
    struct pixel* data;
};

void create_image();
void destroy_image();