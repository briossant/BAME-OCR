#include "ImageProcess.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_surface.h>


Uint32 convolution(SDL_Surface* image, int x, int y, int* kernel, 
        int kernelWidth)
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
            sum_coef += kernel[(j-(y-(kernelWidth/2)))*
                kernelWidth+(i-(x-(kernelWidth/2)))];
            int coef_act = kernel[(j-(y-(kernelWidth/2)))*
                kernelWidth+(i-(x-(kernelWidth/2)))];
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
    
    Uint32* pixtab = image->pixels;

    SDL_Surface *image_converted = SDL_ConvertSurface(image, image->format, 0);

    for (int y = 0; y < height; y++) 
    {
        for (int x = 0; x < width; x++) 
        {

            pixtab[y * width + x] = convolution(image_converted,x,y,kernel,5);
        }
    }

    SDL_FreeSurface(image_converted);

    return;
}
