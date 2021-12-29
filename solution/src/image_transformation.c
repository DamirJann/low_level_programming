#include "../include/image_transformation.h"

/* создаёт копию изображения, которая повёрнута на 90 градусов */
struct image rotate(struct image const source) {
    struct image rotated_image;
    init_image(&rotated_image, source.height, source.width);

    for (uint64_t i = 0; i < source.height; i++) {
        for (uint64_t j = 0; j < source.width; j++) {
            struct pixel source_pixel = get_pixel(source, i, j);
            set_pixel(rotated_image, j, source.height - i - 1, source_pixel);
        }
    }

    return rotated_image;
}
