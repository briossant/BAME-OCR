

#include "ImageProcess.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_surface.h>
#include <err.h>
#include <math.h>
#include <stdlib.h>

#define TRESHOLD 42.42
#define MIN_DIST 32 // set min from image size
#define diagonal_killer 4

// recursive function
int CheckIfDistIsValid(double *points, size_t len, double dist, int i,
                       int count, double *err_max) {
    // printf("pt: %d -- ", points[i]);
    if (count == 10)
        return i;
    double min_err = TRESHOLD * 42;
    double err;
    int min_j = -1;
    for (size_t j = i + 1; j < len; ++j) {
        if ((err = fabs(fabs(points[j] - points[i]) - dist)) <= TRESHOLD)
            if (err < min_err) {
                min_err = err;
                min_j = j;
            }
    }
    if (min_j < 0)
        return -1;
    if (min_err > *err_max)
        *err_max = min_err;
    return CheckIfDistIsValid(points, len, dist, min_j, count + 1, err_max);
}

void SortList(double *list, size_t len) {
    for (size_t i = 0; i < len; ++i) {
        for (size_t j = i + 1; j < len; ++j) {
            if (list[j] < list[i]) {
                double tmp = list[i];
                list[i] = list[j];
                list[j] = tmp;
            }
        }
    }
}

double *GridDetectionAux(double *points, size_t len) {
    SortList(points, len);

    double *res = calloc(3, sizeof(double));
    res[2] = TRESHOLD * 42;
    int founded_alignments = 0;
    for (size_t i = 0; i < len - 9; i++) {
        for (size_t j = i + 1; j < len; j++) {
            double dist = fabs(points[j] - points[i]);
            // printf("\ndist: %d\n", dist);
            if (dist < MIN_DIST) {
                continue;
            }
            int k;
            double err = -1;
            if ((k = CheckIfDistIsValid(points, len, dist, j, 2, &err)) != -1) {
                ++founded_alignments;
                if (err < res[2]) {
                    res[0] = points[i];
                    res[1] = points[k];
                    res[2] = err;
                }
            }
        }
    }
    if (founded_alignments == 0)
        errx(0, "unabled to find a grid");
    printf("founded %d alignments\n", founded_alignments);
    return res;
}

// main function
int *GridDetection(int *lines, int nbr_of_lines) {
    double *vertical = malloc(sizeof(double) * nbr_of_lines);
    double *horizontal = malloc(sizeof(double) * nbr_of_lines);
    size_t v_i = 0;
    size_t h_i = 0;

    for (int i = 0; i < nbr_of_lines * 4; i += 4) {
        // Todo: Check if lines are not very straight (diagonal case)
        if (abs(lines[i] - lines[i + 2]) >
            abs(lines[i + 1] - lines[i + 3]) * diagonal_killer) {
            // Horizontal
            horizontal[h_i++] =
                ((double)lines[i + 1] + (double)lines[i + 3]) / 2;
            // printf("h: %d \n", horizontal[h_i - 1]);
        } else if (abs(lines[i] - lines[i + 2]) * diagonal_killer <
                   abs(lines[i + 1] - lines[i + 3])) {

            // Vertical
            vertical[v_i++] = ((double)lines[i] + (double)lines[i + 2]) / 2;
            // printf("v: %d \n", vertical[v_i - 1]);
        }
    }

    printf("v: %ld , h: %ld \n////// Y /////\n", v_i, h_i);
    double *y = GridDetectionAux(horizontal, h_i);
    printf("\n\ny: %lf %lf\n/////// X //////\n", y[0], y[1]);
    double *x = GridDetectionAux(vertical, v_i);

    printf("\n\nx: %lf %lf\n", x[0], x[1]);
    int *matrix = malloc(sizeof(int) * 4);
    matrix[0] = x[0];
    matrix[1] = y[0];
    matrix[2] = x[1];
    matrix[3] = y[1];

    return matrix;
}
