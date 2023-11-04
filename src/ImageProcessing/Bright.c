#include "ImageProcess.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>


Uint8 fb(Uint8 c, int n)
{
    return (Uint8) (255 * SDL_pow((double) c / 255, n));
}


void Bright(SDL_Surface* image)
{
    int brightness = 3;

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

            r = fb(r,brightness);
            g = fb(g,brightness);
            b = fb(b,brightness);

            // Mettre à jour le pixel
            pixtab[y * width + x] = SDL_MapRGBA(format, r, g, b,a);
        }
    }
    return;
}