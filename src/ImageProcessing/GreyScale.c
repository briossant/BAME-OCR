#include "ImageProcess.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

void GreyScale(char *path)
{
    
    if (IMG_Init(IMG_INIT_JPG|IMG_INIT_PNG) == 0) 
    {
        printf("Error when initializing SDL2_image : %s\n", IMG_GetError());
        return;
    }

    SDL_Surface* image = NULL;
    image = IMG_Load(path);
    //image = SDL_LoadBMP(path);
    if (image == NULL) 
    {
        printf("Can't load image : %s\n", IMG_GetError());
        return;
    }

    printf("Width : %d\nHeight : %d\n", image->w, image->h);
    
    if (SDL_LockSurface(image)<0)
    {
        printf("Can't lock surface : %s\n", SDL_GetError());
        return;
    }


    int height = image->h;
    int width = image->w;
    SDL_PixelFormat* format = image->format;


    for (int y = 0; y < height; y++) 
    {
        for (int x = 0; x < width; x++) 
        {
            //printf("y=%d x=%d pixel=%d\n",y,x,y*image->w+x);
            if (x >= 0 && x < width && y >= 0 && y < height)
            {
                Uint32* pixtab = (Uint32*)image->pixels;
                //Uint32 pixel = pixels[y * image->w + x];
                
                Uint8 r, g, b, a;
                SDL_GetRGBA(pixtab[y * width + x], format, &r, &g, &b, &a);

                // Inverser les composantes RVB
                r = 255 - r;
                g = 255 - g;
                b = 255 - b;

                // Mettre à jour le pixel
                pixtab[y * width + x] = SDL_MapRGBA(format, r, g, b, a);
            }
            else
            {
                printf("Out of image bounds\n");
            }
            
        }
    }

    SDL_UnlockSurface(image);

    if (IMG_SavePNG(image, path) != 0) 
    {
        printf("Error when trying to save the image : %s\n", IMG_GetError());
    }

    SDL_FreeSurface(image);
    IMG_Quit();
    return;
}
