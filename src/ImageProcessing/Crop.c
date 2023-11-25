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

SDL_Surface* Crop(SDL_Surface* image, line l)
{
    int height = image->h;
    int width = image->w;
    const SDL_PixelFormat* format = image->format;
    Uint8 r,g,b,a;
    Uint32* pixtab = image->pixels;

    if (l.x1 > width || l.y1 > height || l.x2 > width || l.y2 >height) // TODO: err msg
        return image;

    int dx = abs(l.x1 - l.x2);
    int dy = abs(l.y1 - l.y2);
    int sx = (l.x1 < l.x2) ? 1 : -1;
    int sy = (l.y1 < l.y2) ? 1 : -1;
    SDL_Surface *image_converted = SDL_CreateRGBSurfaceWithFormat(0, dx, dy, 32, format->format);
    Uint32* new_pixtab = image_converted->pixels;
    SDL_LockSurface(image_converted);

    int ny=0;
    int nx=0;
    for (int y = l.y1; y < l.y2; y += sy)
    {
        nx = 0;
        for (int x = l.x1; x < l.x2; x += sx)
        {
            SDL_GetRGBA(pixtab[y*width+x],format,&r,&g,&b,&a);
            new_pixtab[ny * dx + nx] = SDL_MapRGBA(format, r, g, b, a);
            nx++;
        }
        ny++;
    }
    
    SDL_UnlockSurface(image_converted);
    return image_converted;



















    // int nw = 0;
    // int nh = 0;
    
    // int height = image->h;
    // int width = image->w;
    // const SDL_PixelFormat* format = image->format;

    // Uint8 r,g,b,a;

    // Uint32* pixtab = image->pixels;

    // for (int x = x11+1; x < width; x++) 
    // {
    //     SDL_GetRGBA(pixtab[(x21+1)*width+x],format,&r,&g,&b,&a);
    //     if (r == 255) 
    //     {
    //         break;
    //     }
    //     ++nw;
    // }
    // for (int x = y21+1; x < height; x++) 
    // {
    //     SDL_GetRGBA(pixtab[x*width+x11+1],format,&r,&g,&b,&a);
    //     if (r == 255) 
    //     {
    //         break;
    //     }
    //     ++nh;
    // }

    // SDL_Surface *image_converted = SDL_CreateRGBSurfaceWithFormat(0, nw
    //         , nh, 32, format->format);
    // Uint32* new_pixtab = image_converted->pixels;

    // SDL_LockSurface(image_converted);

    // int ny=0;
    // int nx=0;


    // for (int y = y21+1; y < y21+nw; y++) 
    // {
    //     for (int x = x11+1; x < x11+nh; x++) 
    //     {
    //         SDL_GetRGBA(pixtab[y*width+x], format, &r, &g, &b, &a);
            
    //         new_pixtab[ny * nw + nx] = SDL_MapRGBA(format, r, g, b, a);

    //         ++nx;
    //     }
    //     ++ny;
    //     nx=0;
    // }

    // SDL_UnlockSurface(image_converted);

    // if (IMG_SavePNG(image_converted, filename) != 0) 
    // {
    //     printf("Error when trying to save the image : %s\n", IMG_GetError());
    // }

}
