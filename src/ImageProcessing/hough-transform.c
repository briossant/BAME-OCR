#include "ImageProcess.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_surface.h>
#include <err.h>
#include <math.h>
#include <stdlib.h>

SDL_Surface* hough_transform(SDL_Surface * image, int threshold)
{
    // Init tab
    int* abs = (int*)calloc(image->w, sizeof(int));
    int* ord = (int*)calloc(image->h, sizeof(int));

    // Init SDL var
    Uint8 r,g,b,a;
    Uint32* pixtab = image->pixels;
    const SDL_PixelFormat* format = image->format;

    // Get the lines
    for (int x = 0; x < image->w; x++)
    {
        for (int y = 0; y < image->h; y++)
        {
            SDL_GetRGBA(pixtab[y*image->w + x], format, &r, &g, &b, &a);

            if (r > 200)
            {
                abs[x] += 1;
                ord[y] += 1;
            }
        }
    }

    // Get the coordonates of the lines
    for (int x = 0; x < image->w; x++)
    {
        // printf("abs=%d\n", abs[x]);
        if (abs[x] > threshold)
        {
            draw_line(image, x, 0, x, image->h);
        }
    }

    for (int y = 0; y < image->h; y++)
    {
        // printf("ord=%d\n", ord[y]);
        if (ord[y] > threshold)
        {
            draw_line(image, 0, y, image->w, y);
        }
    }

    free(abs);
    free(ord);

    return image;
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