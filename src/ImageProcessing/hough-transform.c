#include "ImageProcess.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_surface.h>
#include <err.h>
#include <math.h>
#include <stdlib.h>

SDL_Surface* hough_transform(SDL_Surface * image, int pas)
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

            if (r!=0) 
            {
               for (int theta=0; theta<180; theta+=pas) 
               {
                    rho = x*cosf((float)theta) 
                        + y*sinf((float)theta);
                    
                    SDL_GetRGBA(mat[theta*matrice->w+rho], format, &r1
                            , &g1, &b1, &a1);

                    mat[theta*matrice->w+rho] = SDL_MapRGBA(format, r1+2, 
                            g1+2, b1+2, 255);
               }
            } 
        }
    }

    IMG_SavePNG(matrice, "tmp.png");
    
    return matrice;
}

void draw_line(SDL_Surface* image, int x1, int y1, int x2, int y2)
{

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
        if (current_x >=0 && current_x < image->w 
                && current_y >= 0 && current_y < image->h) 
        {
            Uint8 *pixel = (Uint8 *)image->pixels + current_y * image->pitch 
                + current_x * 4;

            *(Uint32 *)pixel = color;
        }

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
}



void draw_hough_line(SDL_Surface* image, SDL_Surface* hough_pic, int seuil)
{ 
    int R = sqrt(image->h*image->h+image->w*image->w);

    const SDL_PixelFormat* format = image->format;

    Uint32* mat = hough_pic->pixels;

    Uint8 r,g,b,a;

    for (int theta = 0; theta<hough_pic->h; ++theta) 
    {
        for (int rho =0; rho<hough_pic->w; ++rho) 
        {
            SDL_GetRGBA(mat[theta*hough_pic->w +rho],format, &r, &g, &b, &a);
            
            if (r>seuil) 
            {
                
                int x1 = rho*cos(theta)-R*sin(theta);
                int y1 = rho*sin(theta)+R*cos(theta);
                int x2 = rho*cos(theta)+R*sin(theta);
                int y2 = rho*sin(theta)-R*cos(theta);
                
    
              /* 
                if (x1<0) 
                {
                    x1 = 0;
                }
                if (x2<0) 
                {
                    x2 = 0;
                }
                if (y1<0) 
                {
                    y1 = 0;
                }
                if (y2<0) 
                {
                    y2 = 0;
                }
*/
                printf("x1= %d  y1= %d  x2= %d  y2= %d\n",x1,y1,x2,y2);


                draw_line(image, x1, y1, x2, y2);
            }
            
        }
    }

}






