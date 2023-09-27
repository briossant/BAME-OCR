#include "ImageProcess.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

/*
static Uint32 blur(SDL_Surface *surface, int i, int j, int n)
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


int kernel[] = {2,4,5,4,2,
                    4,9,12,9,4,
                    5,12,15,12,5,
                    4,9,12,9,4,
                    2,4,5,4,2};

    int kernel2[] = {1,2,1,
                    2,4,2,
                    1,2,1};

    SDL_PixelFormat* format = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888);
    SDL_Surface *image_converted = SDL_ConvertSurface(image, format, 0);
*/

Uint32 convolution(SDL_Surface* image, int x, int y, int* kernel, int kernelWidth)
{
    int sum_coef = 0;
    int sum_r =0 , sum_g =0 , sum_b =0;

    int height = image->h;
    int width = image->w;
    const SDL_PixelFormat* format = image->format;
    Uint32* pixtab = image->pixels;
    Uint8 r,g,b,a;

    for (int j = y-(kernelWidth/2); j <= y+(kernelWidth/2); j++)
    {
        for (int i = x-(kernelWidth/2); i <= x+(kernelWidth/2); i++)
        {
            sum_coef += kernel[(j-(y-(kernelWidth/2)))*kernelWidth+(i-(x-(kernelWidth/2)))];
            int coef_act = kernel[(j-(y-(kernelWidth/2)))*kernelWidth+(i-(x-(kernelWidth/2)))];
            if (j>=0 && j<height && i>=0 && i<width)
            {
                SDL_GetRGBA(pixtab[j*width+i], format, &r, &g, &b, &a);
                sum_r += coef_act*r;
                sum_g += coef_act*g;
                sum_b += coef_act*b;
            }
            
            
        }
    }

    if (sum_coef !=0)
    {
        sum_r /=sum_coef;
        sum_g /=sum_coef;
        sum_b /=sum_coef;
    }

    return SDL_MapRGBA(format, sum_r, sum_g, sum_b, 255);
}

void GaussianBlur(SDL_Surface* image)
{
    int height = image->h;
    int width = image->w;

    int kernel[] = {2,4,5,4,2,
                    4,9,12,9,4,
                    5,12,15,12,5,
                    4,9,12,9,4,
                    2,4,5,4,2};

    SDL_Surface *image_converted = SDL_ConvertSurface(image, image->format, 0);

    for (int y = 0; y < height; y++) 
    {
        for (int x = 0; x < width; x++) 
        {
            //printf("y=%d x=%d pixel=%d\n",y,x,y*image->w+x);
            Uint32* pixtab = image->pixels;

            //pixtab[y * width + x] = blur(image,y,x,blur_radius);
            pixtab[y * width + x] = convolution(image_converted,x,y,kernel,5);
        }
    }
    return;
}