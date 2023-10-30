#include "ImageProcess.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>


Uint8 f(Uint8 c, double n)
{
    if(c <= 255 / 2)
        return (Uint8)( (255/2) * SDL_pow((double) 2 * c / 255, n));
    else
        return 255 - f(255 - c, n);
}


void Contrast(SDL_Surface* image)
{
    double contrast = 2;
    
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

            // Mettre à jour le pixel
            pixtab[y * width + x] = SDL_MapRGBA(format, f(r,contrast), f(g,contrast), f(b,contrast),a);
        }
    }
    return;
}