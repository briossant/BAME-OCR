
#include "ImageProcess.h"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_surface.h>

void ComputeStardizeLens(int width, int height, int *dx, int *dy) {
    if (width > height) {
        *dx = IMAGE_SIZE;
        *dy = IMAGE_SIZE * height / width;
    } else {
        *dx = IMAGE_SIZE * width / height;
        *dy = IMAGE_SIZE;
    }
}

SDL_Surface *StandardizeImage(SDL_Surface *image) {

    SDL_Surface *new_image = SDL_CreateRGBSurfaceWithFormat(
        0, IMAGE_SIZE, IMAGE_SIZE, 32, image->format->format);

    int dx;
    int dy;
    ComputeStardizeLens(image->w, image->h, &dx, &dy);

    SDL_Rect dest = {
        .x = IMAGE_SIZE / 2 - dx / 2,
        .y = IMAGE_SIZE / 2 - dy / 2,
        .w = dx,
        .h = dy,
    };

    SDL_BlitScaled(image, NULL, new_image, &dest);
    return new_image;
}
