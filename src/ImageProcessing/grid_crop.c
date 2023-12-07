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
#include <stdio.h>
#include <stdlib.h>

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
    return accu < height * width - THRESHOLD_VOID_SQUARE;
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

int lessBrightPixel(SDL_Surface *image) {
    int height = image->h;
    int width = image->w;
    const SDL_PixelFormat *format = image->format;
    Uint32 *pixtab = image->pixels;

    int max_x = 0;
    int max_r = 256;
    Uint8 r, g, b, a;

    for (int x = 0; x < height * width; x++) {
        SDL_GetRGBA(pixtab[x], format, &r, &g, &b, &a);
        if (r < max_r) {
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

    const SDL_Rect src = {.x = x1 + OFFSET_FIRST_CROP,
                          .y = y1 + OFFSET_FIRST_CROP,
                          .w = x2 - x1 - OFFSET_FIRST_CROP * 2,
                          .h = y2 - y1 - OFFSET_FIRST_CROP * 2};

    int er = SDL_BlitScaled(image, &src, image_converted, NULL);
    if (er == -1)
        err(0, NULL);

    return image_converted;
}

int End_of_Number(SDL_Surface *image_blured, int coo, int dir_x, int dir_y) {
    int height = image_blured->h;
    int width = image_blured->w;
    Uint32 *pixtab = image_blured->pixels;
    const SDL_PixelFormat *format = image_blured->format;

    int x = coo % width;
    int y = coo / width;
    int res_x = x;
    int res_y = y;

    Uint8 r, g, b, a;

    while (x < width - 1 && x > 0 && y < height - 1 && y > 0) {
        SDL_GetRGBA(pixtab[y * width + x], format, &r, &g, &b, &a);
        x += dir_x;
        y += dir_y;
        if (r > TRESHOLD_NUMBER_DETECTION) {
            res_x = x;
            res_y = y;
        }
    }
    return res_y * width + res_x;
}

SDL_Surface *CenterNumber(SDL_Surface *image) {

    int height = image->h;
    int width = image->w;
    const SDL_PixelFormat *format = image->format;

    SDL_Surface *image_blured =
        SDL_ConvertSurfaceFormat(image, format->format, 0);
    Uint32 *pixtab = image_blured->pixels;

    int kernel[] = {
        5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, //
        5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, //
        5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, //
        5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, //
        5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, //
        5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, //
        5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, //
        5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, //
        5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, //
        5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, //
        5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, //
        5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, //
        5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5  //
    };

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {

            pixtab[y * width + x] = convolution(image, x, y, kernel, 13);
        }
    }

    int nb_center = mostBrightPixel(image_blured);

    int N = End_of_Number(image_blured, nb_center, 0, -1) / width;
    int S = End_of_Number(image_blured, nb_center, 0, 1) / width;
    int W = End_of_Number(image_blured, nb_center, -1, 0) % width;
    int E = End_of_Number(image_blured, nb_center, 1, 0) % width;

    int d2 = E - W;
    int dW = nb_center % width - W;

    int d = S - N;
    int y = N;

    printf("x_center = %i, y_center = %i\n", nb_center % width,
           nb_center / width);

    if (d2 == 0)
        d2 = 1;
    int x = (nb_center % width) - (dW * d) / d2;

    printf("N= %i, S= %i, E= %i, W= %i, d= %i, y= %i, x= %i\n", N, S, E, W, d,
           y, x);

    if (x < OFFSET_CENTER_NUMBER)
        x = OFFSET_CENTER_NUMBER;
    if (x >= width - d - OFFSET_CENTER_NUMBER)
        x = width - d - OFFSET_CENTER_NUMBER - 1;
    if (y < OFFSET_CENTER_NUMBER)
        y = OFFSET_CENTER_NUMBER;
    if (y >= height - d - OFFSET_CENTER_NUMBER)
        y = height - d - OFFSET_CENTER_NUMBER - 1;

    SDL_Surface *image_converted = SDL_CreateRGBSurfaceWithFormat(
        0, NN_IMAGE_SIZE, NN_IMAGE_SIZE, 32, format->format);

    const SDL_Rect src = {.x = x - OFFSET_CENTER_NUMBER,
                          .y = y - OFFSET_CENTER_NUMBER,
                          .w = d + OFFSET_CENTER_NUMBER * 2,
                          .h = d + OFFSET_CENTER_NUMBER * 2};

    int er = SDL_BlitScaled(image, &src, image_converted, NULL);
    if (er == -1)
        err(0, NULL);

    // printf("Is empty? = %i\n",void_square(image_converted));
    return image_converted;
}
