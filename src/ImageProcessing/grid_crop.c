/*
Number of try to write Hough : 3
*/

#include "ImageProcess.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_surface.h>
#include <err.h>
#include <math.h>
#include <stdlib.h>

#define FIRST_CROP_SIZE 16
#define SECOND_CROP_SIZE 28 // mnist size

int void_square(SDL_Surface *image) {
    int accu = 0;
    int height = image->h;
    int width = image->w;
    const SDL_PixelFormat *format = image->format;
    Uint32 *pixtab = image->pixels;

    for (int x = 0; x < height * width; x++) {
        Uint8 r, g, b, a;
        SDL_GetRGBA(pixtab[x], format, &r, &g, &b, &a);
        if (r < 128)
            accu++;
    }
    return accu < height * width - 22;
}

int mostBrightPixel(SDL_Surface *image) {
    int height = image->h;
    int width = image->w;
    const SDL_PixelFormat *format = image->format;
    Uint32 *pixtab = image->pixels;

    int max_x = 0;
    int max_r = -1;
    Uint8 r, g, b, a;

    for (int x = 0; x < height * width; x++) {
        SDL_GetRGBA(pixtab[x], format, &r, &g, &b, &a);
        if (r > max_r) {
            max_r = r;
            max_x = x;
        }
    }
    return max_x;
}

SDL_Surface *Resize_crop(SDL_Surface *image, int x1, int y1, int x2, int y2) {

    int height = image->h;
    int width = image->w;

    if (x2 > width || x1 > width) {
        err(0, "Cordinates of x1 or x2 must be in bounds");
    }
    if (y2 > height || y1 > height) {
        err(0, "Cordinates of y1 or y2 must be in bounds");
    }

    const SDL_PixelFormat *format = image->format;

    SDL_Surface *image_converted = SDL_CreateRGBSurfaceWithFormat(
        0, FIRST_CROP_SIZE, FIRST_CROP_SIZE, 32, format->format);

    const SDL_Rect src = {.x = x1, .y = y1, .w = x2 - x1, .h = y2 - y1};

    int er = SDL_BlitScaled(image, &src, image_converted, NULL);
    if (er == -1)
        err(0, NULL);

    SDL_Surface *image_converted2 = SDL_CreateRGBSurfaceWithFormat(
        0, SECOND_CROP_SIZE, SECOND_CROP_SIZE, 32, format->format);

    GaussianBlur(image_converted);

    int center = mostBrightPixel(image_converted);

    printf("center: %d %d\n", center / FIRST_CROP_SIZE,
           center % FIRST_CROP_SIZE);

    // find the right offset to isolate number
    int offset = 2;
    const SDL_Rect src2 = {.x = x1 + offset,
                           .y = y2 + offset,
                           .w = x2 - x1 - offset * 2,
                           .h = y2 - y1 - offset * 2};

    er = SDL_BlitScaled(image, &src2, image_converted2, NULL);
    if (er == -1)
        err(0, NULL);

    SDL_FreeSurface(image_converted);

    // printf("Is empty? = %i\n",void_square(image_converted));
    return image_converted2;
}
