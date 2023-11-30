#include "ImageProcess.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_surface.h>
#include <err.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

void Crop_grid(SDL_Surface* image, int* lx, int* ly)
{
    pair p1;
    pair p2;
    char s[] = "Grid/Grid00.png";
    SDL_Surface* image_converted;
    for (int y = 0; y < 8; y++)
    {
        for (int x = 0; x < 8; x++)
        {
            p1.x = lx[x];
            p1.y = ly[y];
            p2.x = lx[x + 1];
            p2.y = ly[y + 1];
            
            s[9] = x+'0';
            s[10] = y+'0';
            
            image_converted = Crop(image, p1, p2);

            // Save the crop in the folder Grid/
            if (IMG_SavePNG(image_converted, s) != 0) 
            {
                printf("Error when trying to save the image : %s\n", IMG_GetError());
            }            
        }
    }
    SDL_FreeSurface(image_converted);
}

SDL_Surface* Crop(SDL_Surface* image, pair p1, pair p2)
{
    int height = image->h;
    int width = image->w;
    const SDL_PixelFormat* format = image->format;
    Uint8 r,g,b,a;
    Uint32* pixtab = image->pixels;

    if (p1.x > width || p1.y > height || p2.x > width || p2.y >height)
    {
        printf("error: crop failed\n"); // TODO: improve error message
        return image;
    }

    int dx = abs(p1.x - p2.x);
    int dy = abs(p1.y - p2.y);
    int sx = (p1.x < p2.x) ? 1 : -1;
    int sy = (p1.y < p2.y) ? 1 : -1;
    SDL_Surface *image_converted = SDL_CreateRGBSurfaceWithFormat(0, dx, dy, 32, format->format);
    Uint32* new_pixtab = image_converted->pixels;
    SDL_LockSurface(image_converted);

    int ny=0;
    int nx=0;
    for (int y = p1.y; y < p2.y; y += sy)
    {
        nx = 0;
        for (int x = p1.x; x < p2.x; x += sx)
        {
            SDL_GetRGBA(pixtab[y*width+x],format,&r,&g,&b,&a);
            new_pixtab[ny * dx + nx] = SDL_MapRGBA(format, r, g, b, a);
            nx++;
        }
        ny++;
    }
    
    SDL_UnlockSurface(image_converted);
    return image_converted;
}