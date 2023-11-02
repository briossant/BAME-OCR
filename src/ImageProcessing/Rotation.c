#include "ImageProcess.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>

SDL_Surface* Rotate (SDL_Surface* image, double angle)
{
    int height = image->h;
    int width = image->w;

    double pi = 3.141592;
    

    double c = cos(angle / 180* pi);
    double s = sin(angle / 180* pi);

    int max = 0;
    if (height > width)
    {
        max = height;
    }
    else
    {
        max = width;
    }

    double CenterX = max / 2;
    double CenterY = max / 2;
    
    //SDL_Rect rect = {0,0,max,max};
    //printf("CenterX: %f, CenterY: %f, Cos: %f, Sin: %f\n", CenterX, CenterY, c, s);

    SDL_PixelFormat* format = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888);
    //SDL_Surface *new_image = SDL_ConvertSurface(image, format, 0);
    SDL_Surface *new_image = SDL_CreateRGBSurface(0, max, max, 32, 0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);
    
    for (int y = 0; y < max; y++) 
    {
        double tmp1 = (y - CenterY)*s;
        double tmp2 = (y - CenterY)*c;

        for (int x = 0; x < max; x++) 
        {
            //printf("y=%d x=%d pixel=%d\n",y,x,y*image->w+x);  
            Uint32* pixtab = image->pixels;
            Uint32* new_pixtab = new_image->pixels;

            int i = (x - CenterX)*c - tmp1 + CenterX;
            int j = (x - CenterX)*s + tmp2 + CenterY;

            if (i >= 0 && i < width && j >= 0 && j < height)
            {
                Uint8 r,g,b,a;
                SDL_GetRGBA(pixtab[j*width+i], format, &r, &g, &b, &a);
                new_pixtab[y*max+x] = SDL_MapRGBA(new_image->format, r, g, b, a);
            }
            else
            {
                new_pixtab[y*max+x] = SDL_MapRGBA(new_image->format, 255, 255, 255, 255);
            }
        }
    }

    return new_image;
}