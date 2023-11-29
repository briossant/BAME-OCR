

#include "ImageProcess.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_surface.h>
#include <err.h>
#include <math.h>
#include <stdlib.h>

#define TRESHOLD 4.42
#define MIN_DIST 32 // set min from image size
#define diagonal_killer 6

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
    // to remove in the future
    for (size_t i = 1; i < len; i++) {
        if (points[i - 1] > points[i])
            errx(0, "LIST NOT SORTED OMG");
    }
    double *res = malloc(len * sizeof(double));
    int res_i = 0;
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
                res[res_i] = points[i];
                res[res_i + 1] = points[k];
                res[res_i + 2] = err;
                res_i += 3;
            }
        }
    }
    if (res_i == 0)
        errx(0, "unabled to find a grid");

    int min_i = 0;
    double min_err = res[2];
    printf("pt: %lf %lf %lf\n", res[0], res[1], res[2]);
    for (int i = 3; i < res_i; i += 3) {
        printf("pt: %lf %lf %lf\n", res[i], res[i + 1], res[i + 2]);
        if (res[i + 2] < min_err) {
            min_err = res[i + 2];
            min_i = i;
        }
    }
    // TODO : free and create new tab
    printf("founded grids: %d\n", res_i / 3);
    return res + min_i;
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
