#include "ImageProcess.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>


static Uint32 Gradian2(SDL_Surface *surface, SDL_PixelFormat* format, int i, int j, int height, int width, int n)
{    
    Uint32 *p = surface->pixels;
    Uint8 r,g,b,a;
    
    int sumx = 0;
    int sumy = 0;
    int sum_coef_x = 0;
    int sum_coef_y = 0;

    int covox[] = {
        -1,0,1,
        -2,0,2,
        -1,0,1};
    int covoy[] = {
        1,2,1
        ,0,0,0
        ,-1,-2,-1};
    
    int ib=0, jb=0;

    for (int y = i-n; y < i+n; y++)
    {
        for(int x = j-n; x < j+n; x++)
        {
            sum_coef_y += covoy[ib*3+jb];
            sum_coef_x += covox[ib*3+jb];
            if (x >= 0 && x < width && y >= 0 && y < height){
                SDL_GetRGBA(p[y * width + x], format, &r, &g, &b,&a);
                sumy += covoy[ib*3+jb] * (int)r;
                sumx += covox[ib*3+jb] * (int)r;
            }
            jb++;
        }
        ib++;
        jb=0;
    }

    if (sum_coef_x != 0) sumx = sumx / sum_coef_x;
    if (sum_coef_y != 0) sumy = sumy / sum_coef_y;

    Uint8 grad = (Uint8)sqrt(sumx * sumx + sumy * sumy);
    double direction = atan2(sumy,sumx);
    return grad << 24 | grad << 16 | grad << 8 | 255;
}


static Uint32 Gradian(SDL_Surface *image,SDL_PixelFormat* format, int i, int j, int height, int width)
{
    Uint32* pixtab = image->pixels;

    Uint8 r,g,b,a,bin;

    SDL_GetRGBA(pixtab[i * width + j], format, &r, &g, &b, &a);

    int angle = roundToNearestAngle((int)g);

    switch (angle)
    {
    case 0:
        if (j+1<width)
            SDL_GetRGBA(pixtab[i * width + j+1],format,&g,&bin,&g,&bin);
        else {g = 0;}
        if (j-1>=0)
            SDL_GetRGBA(pixtab[i * width + j-1],format,&b,&bin,&b,&bin);
        else {b = 0;}
        break;

    case 45:
        if (j+1<width && i+1<height)
            SDL_GetRGBA(pixtab[i+1 * width + j+1],format,&g,&bin,&g,&bin);
        else {g = 0;}
        if (j-1>=0 && i-1>=0)
            SDL_GetRGBA(pixtab[i-1 * width + j-1],format,&b,&bin,&b,&bin);
        else {b = 0;}
        break;

    case 90:
        if (i+1<height)
            SDL_GetRGBA(pixtab[i+1 * width + j],format,&g,&bin,&g,&bin);
        else {g = 0;}
        if (i-1>=0)
            SDL_GetRGBA(pixtab[i-1 * width + j],format,&b,&bin,&b,&bin);
        else {b = 0;}
        break;

    case 135:
        if (i+1<height && j-1>=0)
            SDL_GetRGBA(pixtab[i+1 * width + j-1],format,&g,&bin,&g,&bin);
        else {g = 0;}
        if (j+1<width && i-1>=0)
            SDL_GetRGBA(pixtab[i-1 * width + j+1],format,&b,&bin,&b,&bin);
        else {b = 0;}
        break;

    default:
        break;  
    }

    if (r<b || r<g)
    {
        r=0;
    }
    

    return SDL_MapRGBA(format, (Uint8)r, (Uint8)r, (Uint8)r, (Uint8)a);
}


SDL_Surface* Canny (SDL_Surface* image)
{
    int height = image->h;
    int width = image->w;
    SDL_PixelFormat* format = image->format;
    //Uint32* pixtab = image->pixels;

    SDL_Surface* surface_result = SDL_CreateRGBSurfaceWithFormat(0,width,height,32,SDL_PIXELFORMAT_RGBA8888);
    Uint32* new_pixtab = surface_result->pixels;

    for (int y = 0; y < height; y++) 
    {
        for (int x = 0; x < width; x++) 
        {
            Uint32 res = Gradian2(image,format,y,x,height,width,1);
            //printf("result = %x\n",res);
            new_pixtab[y * width + x] = res;
            //new_pixtab[y * width + x] = Gradian(image,format,y,x,height,width);
        }
    }

    return surface_result;
}