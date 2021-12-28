#ifndef GITHUB_ABSTRACT_IMAGE_H
#define GITHUB_ABSTRACT_IMAGE_H
#include <bits/types/FILE.h>
#include <malloc.h>
#include <bits/stdint-uintn.h>

struct pixel { uint8_t b, g, r; };
//TODO implement constant
//static struct pixel DEFAULT_PIXEL = (struct pixel) {.b = 0, .g = 0, .r = 0};


struct image {
    uint64_t width, height;
    struct pixel* data;
};

void init_image(struct image*, uint64_t, uint64_t);
void destroy_image(struct image);


#endif //GITHUB_ABSTRACT_IMAGE_H
