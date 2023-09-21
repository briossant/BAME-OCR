#include "ImageProcess.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>


static Uint32 Gradian2(SDL_Surface *surface, SDL_PixelFormat* format, int i, int j, int height, int width, int n)
{
    const int initial_h = SDL_max(i - n, 0);
    const int initial_w = SDL_max(j - n, 0);
    const int final_h = SDL_min(i + n, height - 1);
    const int final_w = SDL_min(j + n, width - 1);
    Uint32 *p = surface->pixels;
    Uint8 r,g,b,a;
    int sumx = 0;
    int sumy = 0;

    int covox[] = {-1,0,1,-2,0,2,-1,0,1};
    int covoy[] = {-1,-2,-1,0,0,0,1,2,1};
    
    int ib=0, jb=0;

    for (i = initial_h; i < final_h; i++)
    {
        for(j = initial_w; j < final_w; j++)
        {
            SDL_GetRGBA(p[i * width + j], format, &r, &g, &b,&a);
            sumy += covoy[ib*3+jb] * r;
            sumx += covox[ib*3+jb] * r;
            jb++;
        }
        ib++;
        jb=0;
    }

    int grad = sqrt(sumx * sumx + sumy * sumy);
    double direction = atan2(sumy,sumx);
    return SDL_MapRGBA(format, (Uint8)grad, (Uint8)grad, (Uint8)grad,255);
}

int roundToNearestAngle(int angle) {
    int roundedAngle;

    if (angle >= -22.5 && angle < 22.5) {
        roundedAngle = 0;
    } else if (angle >= 22.5 && angle < 67.5) {
        roundedAngle = 45;
    } else if (angle >= 67.5 && angle < 112.5) {
        roundedAngle = 90;
    } else if (angle >= 112.5 && angle < 157.5) {
        roundedAngle = 135;
    } else if (angle >= 157.5 || angle < -157.5) {
        roundedAngle = 0;
    } else if (angle >= -157.5 && angle < -112.5) {
        roundedAngle = 45;
    } else if (angle >= -112.5 && angle < -67.5) {
        roundedAngle = 90;
    } else {
        roundedAngle = 135;
    }

    return roundedAngle;
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
    //application filtre necessaire Canny
    //GreyScale(image);
    //GaussianBlur(image);
    ////////////////////////////////////////////////////////////////


    int height = image->h;
    int width = image->w;
    SDL_PixelFormat* format = image->format;
    //Uint32* pixtab = image->pixels;

    SDL_Surface *image_converted = SDL_ConvertSurface(image, format, 0);
    Uint32* new_pixtab = image_converted->pixels;

    for (int y = 0; y < height; y++) 
    {
        for (int x = 0; x < width; x++) 
        {
            new_pixtab[y * width + x] = Gradian2(image,format,y,x,height,width,1);
            //new_pixtab[y * width + x] = Gradian(image,format,y,x,height,width);
        }
    }
    return image_converted;
}