#include "../include/image_transformation.h"

/* создаёт копию изображения, которая повёрнута на 90 градусов */
struct image rotate(struct image source) {
    struct image rotated_image;
    init_image(&rotated_image, source.height, source.width);

    for (uint64_t i = 0; i < source.height; i++) {
        for (uint64_t j = 0; j < source.width; j++) {
            struct pixel source_pixel = image_get_pixel(source, (struct point) {j, i});
            image_set_pixel(rotated_image, (struct point) {source.height - i - 1, j}, source_pixel);
        }
    }
    return rotated_image;
}
