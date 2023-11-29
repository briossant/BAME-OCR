#include "ImageProcess.h"
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

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
    int min = 255;
    int max = 0;
    size_t count = 0;

    int height = image->h;
    int width = image->w;
    const SDL_PixelFormat *format = image->format;
    Uint32 *pixtab = image->pixels;
    Uint8 r, g, b, a;

    GreyScale(image);

    int count_white = 0;
    int invert = 0;

    for (int x = 0; x < height * width; x++) {
        SDL_GetRGBA(pixtab[x], format, &r, &g, &b, &a);
        if (r > 50) {
            count_white++;
        }
        if (r > max)
            max = r;
        if (r < min)
            min = r;
        count += r;
    }

    int middle = (max + min) / 2;
    middle += count / (width * height);
    middle /= 2;
    //  int threshold = 5;
    if (count_white >
        height * width - count_white) // Black caracter on white background
        invert = 1;

    /*
        size_t size = 242;
        int *queue = malloc(sizeof(int) * size);
        size_t i_current = 0;
        size_t i_insert = 1;
        queue[0] = 0;
        int coo = 0;

        while (i_current < i_insert) {
            int x = queue[i_current] % width;
            int y = queue[i_current] / width;
            coo = queue[i_current];
            ++i_current; // dequeue

            if (x + 1 < width) {
                if (pixtab[coo + 1] > threshold)
                    queue[i_insert++] = coo + 1;
                if (pixtab[coo + 1] > pixtab[max_coo]) {
                    pixtab[max_coo] = 0;
                    max_coo = coo + 1;
                } else
                    pixtab[coo + 1] = 0;
            }

            if (x - 1 >= 0) {
                if (pixtab[coo - 1] > threshold)
                    queue[i_insert++] = coo - 1;
                if (pixtab[coo - 1] > pixtab[max_coo]) {
                    pixtab[max_coo] = 0;
                    max_coo = coo - 1;
                } else
                    pixtab[coo - 1] = 0;
            }

            if (y + width < height) {
                if (pixtab[coo + width] > threshold)
                    queue[i_insert++] = coo + width;
                if (pixtab[coo + width] > pixtab[max_coo]) {
                    pixtab[max_coo] = 0;
                    max_coo = coo + width;
                } else
                    pixtab[coo + width] = 0;
            }
        }
        free(queue);*/

    for (int x = 0; x < height * width; x++) {
        SDL_GetRGBA(pixtab[x], format, &r, &g, &b, &a);

        if (invert && r > middle) {
            r = 255;
            g = 255;
            b = 255;
        } else if (!invert && r <= middle) {
            r = 0;
            g = 0;
            b = 0;
        }

        // Pixel Update
        pixtab[x] = SDL_MapRGBA(format, r, g, b, a);
    }

    if (invert)
        InvertColor(image);
}
