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

    for (int y = 0; y < height; y++) 
    {
        for (int x = 0; x < width; x++) 
        {
            //printf("y=%d x=%d pixel=%d\n",y,x,y*image->w+x);
            Uint32* pixtab = image->pixels;
            //Uint32 pixel = pixels[y * image->w + x];
            
            Uint8 r, g, b, a;
            SDL_GetRGBA(pixtab[y * width + x],format, &r, &g, &b, &a);

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
            

            // Mettre à jour le pixel
            pixtab[y * width + x] = SDL_MapRGBA(format, r, g, b,a);
        }
    }
    return;
}
