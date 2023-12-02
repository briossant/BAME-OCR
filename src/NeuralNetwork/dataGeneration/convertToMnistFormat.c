#include "../network/NeuralNetwork.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

void writeImg(FILE *file, SDL_Surface *img) {
    size_t img_size = img->w * img->h;
    unsigned char *buffer = malloc(img_size * sizeof(unsigned char));

    const SDL_PixelFormat *format = img->format;
    Uint32 *pixtab = img->pixels;
    Uint8 r, g, b, a;
    for (int x = 0; x < img_size; ++x) {
        SDL_GetRGBA(pixtab[x], format, &r, &g, &b, &a);
        buffer[x] = r;
    }

    if (fwrite(buffer, sizeof(unsigned char), img_size, file) != img_size)
        errx(1, "error writing an image");

    free(buffer);
}

void WriteImages(char *path, int32_t magic, int32_t qtt) {
    FILE *file = fopen(path, "w");
    if (file == NULL) {
        errx(1, "error opening file %s", path);
    }

    // 0: magic number ; 1: number of images ; 2: image height ; 3: image width
    uint32_t *headers = malloc(4 * sizeof(uint32_t));
    headers[0] = magic;
    headers[1] = qtt;
    printf("Writting images (magic: %d) -> qtt: %d  h: %d  w: %d\n", headers[0],
           headers[1], headers[2], headers[3]);
    if (fwrite(headers, sizeof(uint32_t), 4, file) != 4)
        errx(1, "error writing images headers");

    fclose(file);
}

void WriteLabels(char *path, uint32_t magic, uint32_t qtt) {
    FILE *file = fopen(path, "w");
    if (file == NULL) {
        errx(1, "error opening file %s", path);
    }

    // 0: magic number ; 1: number of labels
    uint32_t *headers = malloc(2 * sizeof(uint32_t));
    headers[0] = magic;
    headers[1] = qtt;
    printf("Writting labels (magic: %d) -> qtt: %d\n", headers[0], headers[1]);
    if (fwrite(headers, sizeof(uint32_t), 2, file) != 2)
        errx(1, "error writing labels headers");

    fclose(file);
}
