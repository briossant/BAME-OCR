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

SDL_Surface *new_hough_transform(SDL_Surface *image, int delta, int threshold) {

    int height = image->h;
    int width = image->w;
    int h_acc = 2 * sqrt(height * height + width * width);
    int w_acc = 180 + delta;
    const SDL_PixelFormat *format = image->format;

    size_t *accumu = calloc(w_acc * h_acc, sizeof(size_t));

    Uint32 *pixtab = image->pixels;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {

            Uint8 r, g, b, a;
            SDL_GetRGBA(pixtab[y * width + x], format, &r, &g, &b, &a);
            if (r < 127)
                continue;

            for (double theta = 0; theta < w_acc; theta += 1) {
                int raw = (double)x * cos((double)theta / w_acc * M_PI) +
                          (double)y * sin((double)theta / w_acc * M_PI) +
                          (double)h_acc / 2;
                raw >>= 1;
                size_t coo = theta + raw * w_acc;
                accumu[coo]++;
            }
        }
    }

    for (int y = 0; y < h_acc; y++) {
        for (int x = 0; x < w_acc; x++) {
            if (accumu[y * w_acc + x] > threshold) {
                double rho = y * 2 - (double)h_acc / 2,
                       theta = (double)x * M_PI / w_acc;
                double A = cos(theta), B = sin(theta);
                double x0 = A * rho, y0 = B * rho;
                int x1 = (x0 + 10000 * (-B));
                int y1 = (y0 + 10000 * (A));
                int x2 = (x0 - 10000 * (-B));
                int y2 = (y0 - 10000 * (A));
                draw_line(image, x1, y1, x2, y2);
            }
        }
    }
    return image;
}
