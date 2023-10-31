#include "ImageProcess.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_surface.h>
#include <err.h>
#include <math.h>
#include <stdlib.h>

int hough_transform(SDL_Surface * image, int pas)
{
    int R = sqrt(image->h*image->h+image->w*image->w);

    SDL_Surface* matrice = SDL_CreateRGBSurfaceWithFormat(0, R, 180
           , 32, image->format->format);
    
    Uint32* mat = matrice->pixels;

    for (int y = 0; y<matrice->h; ++y) 
    {
        for (int x =0; x<matrice->w; ++x) 
        {
            mat[y*matrice->w + x] = SDL_MapRGBA(image->format, 0, 0, 0, 255);
        }
    }

    Uint32* pixtab = image->pixels;

    const SDL_PixelFormat* format = image->format;

    Uint8 r,g,b,a;

    Uint8 r1,g1,b1,a1;

    int rho = 0;

    float rad = M_PI/180;

    for (int y = 0; y<image->h; ++y) 
    {
        for (int x = 0; x<image->w; ++x) 
        {
            SDL_GetRGBA(pixtab[y*image->w + x], format, &r, &g, &b, &a);

            if (r==255) 
            {
               for (int theta=0; theta<180; theta+=pas) 
               {
                    rho = x*cosf((float)theta*rad) 
                        + (image->h-y)*sinf((float)theta*rad);
                    
                    SDL_GetRGBA(mat[theta*matrice->w+rho], format, &r1
                            , &g1, &b1, &a1);

                    mat[theta*matrice->w+rho] = SDL_MapRGBA(format, r1+1, 
                            g1+1, b1+1, 255);
               }
            } 
        }
    }

    if (IMG_SavePNG(matrice, "hough_transform.png") != 0) 
    {
        printf("Error when trying to save the image : %s\n", IMG_GetError());
    }

    return 1;
}
