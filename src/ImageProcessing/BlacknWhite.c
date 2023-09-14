#include "ImageProcess.h"
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

void BlacknWhite(SDL_Surface* image)
{
    int height = image->h;
    int width = image->w;
    const SDL_PixelFormat* format = image->format;

    for (int y = 0; y < height; y++) 
    {
        for (int x = 0; x < width; x++) 
        {
            //printf("y=%d x=%d pixel=%d\n",y,x,y*image->w+x);
            Uint32* pixtab = image->pixels;
            //Uint32 pixel = pixels[y * image->w + x];
            
            Uint8 r, g, b, a;
            SDL_GetRGBA(pixtab[y * width + x],format, &r, &g, &b, &a);

            // Inverser les composantes RVB
            r = 255 - r;
            g = 255 - g;
            b = 255 - b;

            // Mettre à jour le pixel
            pixtab[y * width + x] = SDL_MapRGBA(format, r, g, b,a);
        }
    }
}
