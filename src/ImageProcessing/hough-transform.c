#include "ImageProcess.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_surface.h>
#include <err.h>
#include <math.h>
#include <stddef.h>
#include <stdlib.h>

#define THRESHOLD 0.22

void draw_line(SDL_Surface *image, int x1, int y1, int x2, int y2,
               Uint32 color) {

    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx, sy;

    if (x1 < x2)
        sx = 1;
    else
        sx = -1;

    if (y1 < y2)
        sy = 1;
    else
        sy = -1;

    int err = dx - dy;
    int current_x = x1;
    int current_y = y1;

    while (current_x != x2 || current_y != y2) {
        if (current_x >= 0 && current_x < image->w && current_y >= 0 &&
            current_y < image->h) {
            Uint8 *pixel = (Uint8 *)image->pixels + current_y * image->pitch +
                           current_x * 4;

            *(Uint32 *)pixel = color;
        }

        int err2 = 2 * err;
        if (err2 > -dy) {
            err -= dy;
            current_x += sx;
        }
        if (err2 < dx) {
            err += dx;
            current_y += sy;
        }
    }
}

void LocalMaximum(int *accumu, int h_acc, int w_acc, int coo, int threshold) {
    size_t size = 24200;
    int *queue = malloc(sizeof(int) * size);
    size_t i_current = 0;
    size_t i_insert = 1;
    queue[0] = coo;
    int max_coo = coo;

    // check for buffer overflow
    while (i_current < i_insert) {
        int x = queue[i_current] % w_acc;
        int y = queue[i_current] / w_acc;
        coo = queue[i_current];
        ++i_current; // dequeue

        if (x + 1 < w_acc) {
            if (accumu[coo + 1] > threshold)
                queue[i_insert++] = coo + 1;
            if (accumu[coo + 1] > accumu[max_coo]) {
                accumu[max_coo] = 0;
                max_coo = coo + 1;
            } else
                accumu[coo + 1] = 0;
        }

        if (x - 1 >= 0) {
            if (accumu[coo - 1] > threshold)
                queue[i_insert++] = coo - 1;
            if (accumu[coo - 1] > accumu[max_coo]) {
                accumu[max_coo] = 0;
                max_coo = coo - 1;
            } else
                accumu[coo - 1] = 0;
        }

        if (y + w_acc < h_acc) {
            if (accumu[coo + w_acc] > threshold)
                queue[i_insert++] = coo + w_acc;
            if (accumu[coo + w_acc] > accumu[max_coo]) {
                accumu[max_coo] = 0;
                max_coo = coo + w_acc;
            } else
                accumu[coo + w_acc] = 0;
        }
    }
    accumu[max_coo] = -accumu[max_coo];
    free(queue);
}

double *hough_transform(SDL_Surface *image, int *return_size) {

    int height = image->h;
    int width = image->w;
    int h_acc = 2 * sqrt(height * height + width * width);
    int w_acc = 180;
    const SDL_PixelFormat *format = image->format;

    int *accumu = calloc(w_acc * h_acc, sizeof(size_t));

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

    int max = accumu[0];
    for (int y = 0; y < h_acc * w_acc; y++) {
        if (accumu[y] > max) {
            max = accumu[y];
        }
    }
    int threshold = max * THRESHOLD;
    printf("Hough threshold = %i ; max = %i\n", threshold, max);

    size_t nb_droite = 0;
    for (int y = 0; y < h_acc * w_acc; y++) {
        if (accumu[y] > threshold) {
            // LocalMaximum(accumu, h_acc, w_acc, y, threshold);
            ++nb_droite;
        }
    }

    double *matrix = calloc(nb_droite * 2, sizeof(double));
    size_t i = 0;
    for (int y = 0; y < h_acc; y++) {
        for (int x = 0; x < w_acc; x++) {
            if (accumu[y * w_acc + x] > threshold) {
                // if (accumu[y * w_acc + x] < -threshold) {
                double rho = y * 2 - (double)h_acc / 2,
                       theta = (double)x * M_PI / w_acc;
                matrix[i++] = rho;
                matrix[i++] = theta;
                /*
                 double A = cos(theta), B = sin(theta);
                 double x0 = A * rho, y0 = B * rho;
                 matrix[i++] = (x0 + h_acc * (-B));
                 matrix[i++] = (y0 + h_acc * (A));
                 matrix[i++] = (x0 - h_acc * (-B));
                 matrix[i++] = (y0 - h_acc * (A));
 */
                // Uint32 color = SDL_MapRGBA(image->format, 255, 0, 0, 255);
                // draw_line(image, matrix[i - 4], matrix[i - 3], matrix[i - 2],
                //           matrix[i - 1], color);
                //  test of Grid Detection
            }
        }
    }
    *return_size = nb_droite;
    return matrix;
}

int *TransformHoughPolarToPoints(double *hough_matrix, size_t len) {

    int *matrix = calloc(len * 4, sizeof(int));
    int i = 0;
    for (int y = 0; y < len * 2; y += 2) {
        double rho = hough_matrix[y], theta = hough_matrix[y + 1];
        double A = cos(theta), B = sin(theta);
        double x0 = A * rho, y0 = B * rho;
        matrix[i++] = (x0 + 1000 * (-B));
        matrix[i++] = (y0 + 1000 * (A));
        matrix[i++] = (x0 - 1000 * (-B));
        matrix[i++] = (y0 - 1000 * (A));
        // Uint32 color = SDL_MapRGBA(image->format, 255, 0, 0, 255);
        // draw_line(image, matrix[i - 4], matrix[i - 3], matrix[i - 2],
        //           matrix[i - 1], color);
        //  test of Grid Detection
    }

    return matrix;
}
