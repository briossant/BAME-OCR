#include "ImageProcess.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>
#include <math.h>

int convolution_grayscale(SDL_Surface* image, int x, int y, int* kernel, int kernelWidth)
{
    int sum_coef = 0;
    int sum_r =0 ;

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
                sum_r += coef_act*(int)r;
            }
            
            
        }
    }
    if (sum_coef != 0)
        sum_r /=sum_coef;

    return sum_r;
}

SDL_Surface *Intensity_Gradian(SDL_Surface *image)
{
    int height = image->h;
    int width = image->w;
    const SDL_PixelFormat* format = image->format;

    SDL_Surface *image_converted = SDL_CreateRGBSurfaceWithFormat(0, width, height, 32, format->format);

    //Uint32* pixtab = image->pixels;
    Uint32* new_pixtab = image_converted->pixels;

    int grad_x = 0;
    int grad_y = 0;
    int grad=0;

    int grad_x_kernel[] = {-1,0,1,
                           -2,0,2,
                           -1,0,1};

    int grad_y_kernel[] = {-1,-2,-1,
                           0,0,0,
                           1,2,1};

    for (int y = 0; y < height; y++) 
    {
        for (int x = 0; x < width; x++) 
        {
            grad_x = convolution_grayscale(image, x, y, grad_x_kernel, 3);
            grad_y = convolution_grayscale(image, x, y, grad_y_kernel, 3);

            grad = sqrt(grad_x*grad_x + grad_y*grad_y);

            new_pixtab[y * width + x] = SDL_MapRGBA(format, (Uint8)grad, (Uint8)grad, (Uint8)grad, 255);
        }
    }

    return image_converted;
}