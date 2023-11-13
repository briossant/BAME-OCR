#include "ImageProcess.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_surface.h>
#include <err.h>
#include <math.h>
#include <stdlib.h>

int** hough_matrix(SDL_Surface * image, int delta)
{
    int rho;
    int R = sqrt(image->h*image->h+image->w*image->w);
    int** Hough = (int**)malloc(R*sizeof(int**));
    for (int i = 0; i < R; i++)
        Hough[i] = (int*)calloc(180, sizeof(int*));

    for (int x = 0; x < image->w; x++)
    {
        for (int y = 0; y < image->h; y++)
        {
            for(int tetha = 0; tetha < 180; tetha += delta)
            {
                rho = x * cos(tetha) + y * sin(tetha);
                if (rho >= 0)
                    Hough[rho][tetha] += 1;
            }
        }
    }
    return Hough;
}

SDL_Surface* draw_line2(SDL_Surface * image, int rho, int tetha)
{
    const SDL_PixelFormat* format = image->format;
    Uint32* pixtab = image->pixels;
    int y;
    for (int x = 0; x < image->w; x++)
    {
        y = (rho - x * cos(tetha)) / sin(tetha);

        if (y > image->h)
            printf("%d\n", y);

        if (y >= 0)
            pixtab[y * image->w + x] = SDL_MapRGBA(format, 255, 0, 0, 255); // FIXME: seg fault
    }

    return image;
}

SDL_Surface* new_hough_transform(SDL_Surface * image, int delta, int threshold)
{
    int** Hought = hough_matrix(image, delta);

    int R = sqrt(image->h*image->h+image->w*image->w);

    for (int rho = 0; rho < R; rho++)
    {
        for (int tetha = 0; tetha < 180; tetha++)
        {
            if (Hought[rho][tetha] > threshold)
            {
                image = draw_line2(image, rho, tetha);
            }
        }
    }

    printf("ok\n");
    
    return image;
}