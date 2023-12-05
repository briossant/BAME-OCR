#include "ImageProcess.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

SDL_Surface *Rotate(SDL_Surface *image, double angle) {
    if (abs(angle) < 0.01)
        return image;
    int height = image->h;
    int width = image->w;

    double pi = 3.141592;

    double c = cos(angle / 180 * pi);
    double s = sin(angle / 180 * pi);

    int max = sqrt(height * height + width * width);

    double CenterX = max / 2.0;
    double CenterY = max / 2.0;

    // SDL_Rect rect = {0,0,max,max};
    // printf("CenterX: %f, CenterY: %f, Cos: %f, Sin: %f\n", CenterX, CenterY,
    // c, s);

    SDL_PixelFormat *format = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888);
    // SDL_Surface *new_image = SDL_ConvertSurface(image, format, 0);
    SDL_Surface *new_image = SDL_CreateRGBSurface(
        0, max, max, 32, 0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);

    for (int y = 0; y < max; y++) {
        double tmp1 = (y - CenterY) * s;
        double tmp2 = (y - CenterY) * c;

        for (int x = 0; x < max; x++) {
            // printf("y=%d x=%d pixel=%d\n",y,x,y*image->w+x);
            Uint32 *pixtab = image->pixels;
            Uint32 *new_pixtab = new_image->pixels;

            int i = (x - CenterX) * c - tmp1 + CenterX;
            int j = (x - CenterX) * s + tmp2 + CenterY;

            if (i >= 0 && i < width && j >= 0 && j < height) {
                Uint8 r, g, b, a;
                SDL_GetRGBA(pixtab[j * width + i], format, &r, &g, &b, &a);
                new_pixtab[y * max + x] =
                    SDL_MapRGBA(new_image->format, r, g, b, a);
            } else {
                new_pixtab[y * max + x] =
                    SDL_MapRGBA(new_image->format, 255, 255, 255, 255);
            }
        }
    }

    return new_image;
}

double GetImageAngle(int *matrix_hough, size_t len) {
    double *angle_matrix = malloc(sizeof(double) * len);

    int angle_matrix_i = 0;

    double angle, dx, dy;
    for (int i = 0; i < len * 4; i += 4) {
        dx = matrix_hough[i + 2] - matrix_hough[i];
        dy = matrix_hough[i + 3] - matrix_hough[i + 1];

        if (dx == 0) {
            angle = M_PI_2;
            if (dy < 0)
                angle *= -1;
        } else
            angle = atan(dy / dx);

        // edge case /!\ will break if grid is at perfect PI/2 angle
        if (angle <= -M_PI_4 || angle > M_PI_4) {
            if (angle < 0)
                angle += M_PI_2;
            angle_matrix[angle_matrix_i++] = angle;
        }
    }
    SortList(angle_matrix, angle_matrix_i);

    return angle_matrix[angle_matrix_i / 2];
}

double GetImageAngleAndRotateHoughMatrix(double *matrix_hough, size_t len) {

    int *MatrixPoints = TransformHoughPolarToPoints(matrix_hough, len);

    double angle = GetImageAngle(MatrixPoints, len);
    for (int i = 1; i < len; i += 2) {
        matrix_hough[i] += angle;
    }

    return angle * 180 / M_PI;
}
