#include "ImageProcess.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

static Uint32 blur2(SDL_Surface *surface, int i, int j, int n)
{
    const int initial_h = SDL_max(i - n, 0);
    const int initial_w = SDL_max(j - n, 0);
    const int final_h = SDL_min(i + n, surface->h - 1);
    const int final_w = SDL_min(j + n, surface->w - 1);
    const int nb_pixel = ((final_h - initial_h) * (final_w - initial_w));
    const Uint32 *p = surface->pixels;

    Uint32 sum_r = 0, sum_g = 0, sum_b = 0;
    SDL_Color color;

    for (i = initial_h; i < final_h; i++)
        for(j = initial_w; j < final_w; j++)
        {
            SDL_GetRGB(p[i * surface->w + j], surface->format, &color.r, &color.g, &color.b);
            sum_r += color.r;
            sum_g += color.g;
            sum_b += color.b;
        }

    return SDL_MapRGB(surface->format, sum_r / nb_pixel, sum_g / nb_pixel, sum_b / nb_pixel);
}

void ArroundGaussianBlur(SDL_Surface* image)
{
    int blur_radius = 2;

    int height = image->h;
    int width = image->w;

    for (int y = 0; y < height; y++) 
    {
        for (int x = 0; x < width; x++) 
        {
            //printf("y=%d x=%d pixel=%d\n",y,x,y*image->w+x);
            Uint32* pixtab = image->pixels;

            pixtab[y * width + x] = 255 - SDL_abs(pixtab[y * width + x] - blur2(image, y, x, blur_radius));
        }
    }
    return;
}