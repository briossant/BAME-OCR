#include "ImageProcess.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

void GreyScale(SDL_Surface* image)
{
    //printf("Width : %d\nHeight : %d\n", image->w, image->h);
    //printf("format : %s\n", SDL_GetPixelFormatName(image->format->format));

    
    int height = image->h;
    int width = image->w;
    const SDL_PixelFormat* format = image->format;

    Uint32* pixtab = image->pixels;


    for (int y = 0; y < height; y++) 
    {
        for (int x = 0; x < width; x++) 
        {
            //printf("y=%d x=%d pixel=%d\n",y,x,y*image->w+x);
            //Uint32* pixtab = image->pixels;
            //Uint32 pixel = pixels[y * image->w + x];
            
            Uint8 r, g, b, a;
            SDL_GetRGBA(pixtab[y * width + x],format, &r, &g, &b, &a);

            Uint8 gris = (r + g + b) / 3;

            // Mettre à jour le pixel
            pixtab[y * width + x] = SDL_MapRGBA(format, gris, gris, gris,a);
        }
    }
    /*
    if (IMG_SavePNG(image_converted, path) != 0) 
    {
        printf("Error when trying to save the image : %s\n", IMG_GetError());
    }

    SDL_FreeSurface(image_converted);
    IMG_Quit();*/
    return;
}
