#include "ImageProcess.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

void GreyScale(char *path)
{
    if (IMG_Init(IMG_INIT_PNG) == 0) 
    {
    printf("Error when initializing SDL2_image : %s\n", IMG_GetError());
    }
    
    struct SDL_Surface* image = IMG_Load(path);
    if (!image) 
    {
    printf("Can't load image : %s\n", IMG_GetError());
    }

    for (int y = 0; y < image->h; ++y) 
    {
        for (int x = 0; x < image->w; ++x) 
        {
            Uint32* pixels = (Uint32*)image->pixels;
            Uint32 pixel = pixels[y * image->w + x];

            Uint8 r, g, b, a;
            SDL_GetRGBA(pixel, image->format, &r, &g, &b, &a);

            // Inverser les composantes RVB
            r = 255 - r;
            g = 255 - g;
            b = 255 - b;

            // Mettre à jour le pixel
            pixel = SDL_MapRGBA(image->format, r, g, b, a);
            pixels[y * image->w + x] = pixel;
        }
    }
    
    if (IMG_SavePNG(image, path) != 0) 
    {
        printf("Error when trying to save the image : %s\n", IMG_GetError());
    }


    SDL_FreeSurface(image);
    IMG_Quit();
}
