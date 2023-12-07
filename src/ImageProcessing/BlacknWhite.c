#include "ImageProcess.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <complex.h>
#include <stdio.h>
#include <stdlib.h>

void BlacknWhite(SDL_Surface *image) {
    Uint8 biais = 127;

    int height = image->h;
    int width = image->w;
    const SDL_PixelFormat *format = image->format;
    Uint32 *pixtab = image->pixels;
    Uint8 r, g, b, a;

    for (int x = 0; x < width * height; x++) {
        SDL_GetRGBA(pixtab[x], format, &r, &g, &b, &a);

        if ((r + g + b) / 3 > biais) {
            r = 255;
            g = 255;
            b = 255;
        } else {
            r = 0;
            g = 0;
            b = 0;
        }

        // Pixel Update
        pixtab[x] = SDL_MapRGBA(format, r, g, b, a);
    }
}

void InvertColor(SDL_Surface *image) {
    int height = image->h;
    int width = image->w;
    const SDL_PixelFormat *format = image->format;
    Uint32 *pixtab = image->pixels;
    Uint8 r, g, b, a;

    for (int x = 0; x < height * width; x++) {
        SDL_GetRGBA(pixtab[x], format, &r, &g, &b, &a);

        r = 255 - r;
        g = 255 - g;
        b = 255 - b;

        // Pixel Update
        pixtab[x] = SDL_MapRGBA(format, r, g, b, a);
    }
}

void Balance(SDL_Surface *image) {
    // int min = 255;
    // int max = 0;
    // size_t count = 0;

    int height = image->h;
    int width = image->w;
    const SDL_PixelFormat *format = image->format;
    Uint32 *pixtab = image->pixels;
    Uint8 r, g, b, a;
    GreyScale(image);

    int count_white = 0;

    int max_coo = mostBrightPixel(image);
    int min_coo = lessBrightPixel(image);

    Uint8 max_pix;
    Uint8 min_pix;
    SDL_GetRGBA(pixtab[max_coo], format, &max_pix, &g, &b, &a);
    SDL_GetRGBA(pixtab[min_coo], format, &min_pix, &g, &b, &a);

    Uint8 mid_pix = (max_pix - min_pix) / 2 + min_pix;
    for (int x = 0; x < height * width; x++) {
        SDL_GetRGBA(pixtab[x], format, &r, &g, &b, &a);
        if (r > mid_pix)
            count_white++;
    }
    if (count_white > height * width - count_white) {
        InvertColor(image);

        SDL_GetRGBA(pixtab[min_coo], format, &max_pix, &g, &b, &a);
        SDL_GetRGBA(pixtab[max_coo], format, &min_pix, &g, &b, &a);
        mid_pix = (double)(max_pix - min_pix) / THRESHOLD_BALANCE + min_pix;
    }
    if (max_pix - min_pix < BALANCE_MIN_DIFF)
        mid_pix = 255;

    if (max_pix == 0)
        return;
    double whiting_factor = 255. / max_pix;
    printf("max %d - min %d - mid %d - factor %lf\n", max_pix, min_pix, mid_pix,
           whiting_factor);
    for (int x = 0; x < width * height; ++x) {
        SDL_GetRGBA(pixtab[x], format, &r, &g, &b, &a);
        if (r <= mid_pix) {
            pixtab[x] = SDL_MapRGBA(format, 0, 0, 0, 255);
        } else {
            r = (double)r * whiting_factor;
            if (r <= min_pix)
                r = 255;
            pixtab[x] = SDL_MapRGBA(format, r, r, r, 255);
        }
    }
}
