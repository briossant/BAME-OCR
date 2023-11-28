#include "ImageProcess.h"
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

void BlacknWhite(SDL_Surface* image)
{
    Uint8 biais = 127;

    int height = image->h;
    int width = image->w;
    const SDL_PixelFormat* format = image->format;
    Uint32* pixtab = image->pixels;
    Uint8 r, g, b, a;

    for (int x = 0; x < width*height; x++) 
    {
        SDL_GetRGBA(pixtab[x],format, &r, &g, &b, &a);

        if ((r + g + b)/3 > biais)
        {
            r = 255;
            g = 255;
            b = 255;
        }
        else
        {
            r = 0;
            g = 0;
            b = 0;
        }
        
        // Pixel Update
        pixtab[x] = SDL_MapRGBA(format, r, g, b,a);
    }
}

void InvertColor(SDL_Surface* image)
{
    int height = image->h;
    int width = image->w;
    const SDL_PixelFormat* format = image->format;
    Uint32* pixtab = image->pixels;
    Uint8 r, g, b, a;

    for (int x = 0; x < height*width; x++) 
    {
        SDL_GetRGBA(pixtab[x],format, &r, &g, &b, &a);

        r = 255-r;
        g = 255-g;
        b = 255-b;

        // Pixel Update
        pixtab[x] = SDL_MapRGBA(format, r, g, b,a);
    }
}

void Balance(SDL_Surface* image)
{
    //TODO: Need to compute the average of the most black and the most white pixels to choose a threshold

    int height = image->h;
    int width = image->w;
    const SDL_PixelFormat* format = image->format;
    Uint32* pixtab = image->pixels;
    Uint8 r, g, b, a;

    GreyScale(image);

    int count_white = 0;
    int invert = 0;

    for (int x = 0; x < height*width; x++) 
    {
        SDL_GetRGBA(pixtab[x],format, &r, &g, &b, &a);
        if (r > 50 || g > 50 || b > 50)
        {
            count_white++;
        }
    }

    if (count_white > height*width - count_white) // Black caracter on white background
        invert = 1;


    for (int x = 0; x < height*width; x++)
    {
        SDL_GetRGBA(pixtab[x],format, &r, &g, &b, &a);

        if (invert && (r > 50 || g > 50 || b > 50))
        {
            r = 255;
            g = 255;
            b = 255;
        }
        else if (!invert && (r <= 50 || g <= 50 || b <= 50))
        {
            r = 0;
            g = 0;
            b = 0;
        }

        // Pixel Update
        pixtab[x] = SDL_MapRGBA(format, r, g, b,a);
    }

    if (invert)
        InvertColor(image);
}
