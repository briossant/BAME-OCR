#include "ImageProcess.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_surface.h>
#include <err.h>
#include <math.h>
#include <stdlib.h>

SDL_surface* hough_transform(SDL_Surface * image, int pas)
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

    float rad = (M_PI/180);

    for (int y = 0; y<image->h; ++y) 
    {
        for (int x = 0; x<image->w; ++x) 
        {
            SDL_GetRGBA(pixtab[y*image->w + x], format, &r, &g, &b, &a);

            if (r==255) 
            {
               for (int theta=0; theta<180; ++theta) 
               {
                    rho = x*cosf((float)((theta))*rad) 
                        + y*sinf((float)((theta))*rad);
                    
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

void draw_line(SDL_Surface* image, int x1, int y1, int x2, int y2)
{
    SDL_LockSurface(image);

    Uint32 color = SDL_MapRGBA(image->format, 255, 0, 0, 255);

    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx, sy;

    if (x1 < x2) 
    {
        sx = 1;
    } 
    else 
    {
        sx = -1;
    }

    if (y1 < y2) 
    {
        sy = 1;
    } 
    else 
    {
        sy = -1;
    }

    int err = dx - dy;
    int current_x = x1;
    int current_y = y1;

    while (current_x != x2 || current_y != y2) 
    {
        Uint8 *pixel = (Uint8 *)image->pixels + current_y * image->pitch 
            + current_x * 4;

        *(Uint32 *)pixel = color;

        int err2 = 2 * err;
        if (err2 > -dy) 
        {
            err -= dy;
            current_x += sx;
        }
        if (err2 < dx) 
        {
            err += dx;
            current_y += sy;
        }
    }

    SDL_UnlockSurface(image);
}

void draw_hough_line(SDL_Surface* image, SDL_Surface* hough_pic, int seuil)
{ 
    int R = sqrt(image->h*image->h+image->w*image->w);

    const SDL_PixelFormat* format = image->format;

    Uint32* mat = hough_pic->pixels;

    Uint8 r,g,b,a;

    for (int y = 0; y<hough_pic->h; ++y) 
    {
        for (int x =0; x<hough_pic->w; ++x) 
        {
            SDL_GetRGBA(mat[y*hough_pic->w +x],format, &r, &g, &b, &a);
            
            if (r>seuil) 
            {
                
            }
            
            mat[y*hough_pic->w + x] = SDL_MapRGBA(image->format, 0, 
                    0, 0, 255);
        }
    }

}






