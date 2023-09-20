#include "ImageProcess.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>

static Uint32 Gradian(SDL_Surface *image,const SDL_PixelFormat* format, int i, int j, int height, int width)
{
    Uint32* pixtab = image->pixels;

    Uint8 r,g,b,a,bin;
    double gradx,grady,grad;

    if (j+1<height)
    {
        SDL_GetRGBA(pixtab[i * width + j+1],format,&r,&r,&r,&bin);
    }
    else {r = 0;}
    if (j-1>=0)
    {
        SDL_GetRGBA(pixtab[i * width + j-1],format,&g,&g,&g,&bin);
    }
    else {g = 0;}
    if (i+1<width)
    {
        SDL_GetRGBA(pixtab[i+1 * width + j],format,&b,&b,&b,&bin);
    }
    else {b = 0;}
    if (i-1>=0)
    {
        SDL_GetRGBA(pixtab[i-1 * width + j],format,&a,&a,&a,&bin);
    }
    else {a = 0;}

    gradx = r + (g*-1);
    grady = b + (a*-1);

    grad = sqrt(gradx*gradx + grady*grady);

    return SDL_MapRGBA(format, (Uint8)grad, (Uint8)grad, (Uint8)grad, 255);
}


void Canny (SDL_Surface* image)
{
    //application filtre necessaire Canny
    //GreyScale(image);
    //GaussianBlur(image);
    ////////////////////////////////////////////////////////////////


    int height = image->h;
    int width = image->w;
    const SDL_PixelFormat* format = image->format;
    Uint32* pixtab = image->pixels;

    for (int y = 0; y < height; y++) 
    {
        for (int x = 0; x < width; x++) 
        {
            pixtab[y * width + x] = Gradian(image,format,y,x,height,width);
        }
    }
    return;
}