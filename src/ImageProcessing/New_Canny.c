#include "ImageProcess.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>
#include <math.h>

int convolution_grayscale(SDL_Surface* image, int x, int y, int* kernel, int kernelWidth)
{
    int sum_coef = 0;
    int sum_r =0 ;

    int height = image->h;
    int width = image->w;
    const SDL_PixelFormat* format = image->format;
    Uint32* pixtab = image->pixels;
    Uint8 r,g,b,a;

    for (int j = y-(kernelWidth/2); j <= y+(kernelWidth/2); j++)
    {
        for (int i = x-(kernelWidth/2); i <= x+(kernelWidth/2); i++)
        {
            sum_coef += kernel[(j-(y-(kernelWidth/2)))*kernelWidth+(i-(x-(kernelWidth/2)))];
            int coef_act = kernel[(j-(y-(kernelWidth/2)))*kernelWidth+(i-(x-(kernelWidth/2)))];
            if (j>=0 && j<height && i>=0 && i<width)
            {
                SDL_GetRGBA(pixtab[j*width+i], format, &r, &g, &b, &a);
                sum_r += coef_act*(int)r;
            }
            
            
        }
    }
    if (sum_coef != 0)
        sum_r /=sum_coef;

    return sum_r;
}

SDL_Surface *Intensity_Gradian(SDL_Surface *image)
{
    int height = image->h;
    int width = image->w;
    const SDL_PixelFormat* format = image->format;

    SDL_Surface *image_converted = SDL_CreateRGBSurfaceWithFormat(0, width, height, 32, format->format);

    //Uint32* pixtab = image->pixels;
    Uint32* new_pixtab = image_converted->pixels;

    int grad_x = 0;
    int grad_y = 0;
    int grad=0;

    int grad_x_kernel[] = {-1,0,1,
                           -2,0,2,
                           -1,0,1};

    int grad_y_kernel[] = {-1,-2,-1,
                           0,0,0,
                           1,2,1};

// possible ajustement des bornes pour plus ou moins de precision sur les bords de l'image

    for (int y = 2; y < height-2; y++) 
    {
        for (int x = 2; x < width-2; x++) 
        {
            grad_x = convolution_grayscale(image, x, y, grad_x_kernel, 3)/4;
            grad_y = convolution_grayscale(image, x, y, grad_y_kernel, 3)/4;

            grad = sqrt(grad_x*grad_x + grad_y*grad_y);

            new_pixtab[y * width + x] = SDL_MapRGBA(format, (Uint8)grad, (Uint8)grad, (Uint8)grad, 255);
        }
    }

    return image_converted;
}

SDL_Surface* Orientation_Gradian (SDL_Surface* image)
{
    int height = image->h;
    int width = image->w;
    const SDL_PixelFormat* format = image->format;

    SDL_Surface *image_converted = SDL_CreateRGBSurfaceWithFormat(0, width, height, 32, format->format);

    //Uint32* pixtab = image->pixels;
    Uint32* new_pixtab = image_converted->pixels;

    int grad_x = 0;
    int grad_y = 0;
    int grad=0;

    int grad_x_kernel[] = {-1,0,1,
                           -2,0,2,
                           -1,0,1};

    int grad_y_kernel[] = {-1,-2,-1,
                           0,0,0,
                           1,2,1};

    for (int y = 0; y < height; y++) 
    {
        for (int x = 0; x < width; x++) 
        {
            grad_x = convolution_grayscale(image, x, y, grad_x_kernel, 3);
            grad_y = convolution_grayscale(image, x, y, grad_y_kernel, 3);

            grad = atan2(grad_x, grad_y);

            new_pixtab[y * width + x] = SDL_MapRGBA(format, (Uint8)grad, (Uint8)grad, (Uint8)grad, 255);
        }
    }

    return image_converted;
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

static Uint32 Sup_max_calculus(Uint8 Intensity_Gradian,Uint8 Orientation_Gradian, SDL_Surface* surface, int i, int j)
{
    Uint32* pixtab = surface->pixels;
    int height = surface->h;
    int width = surface->w;
    SDL_PixelFormat* format = surface->format;

    int angle = roundToNearestAngle((int)Orientation_Gradian);

    Uint8 bin, intensity1=0 ,intensity2=0;

    switch (angle)
    {
    case 0:
        if (i+1<width)
            SDL_GetRGBA(pixtab[j * width + i+1],format,&intensity1,&bin,&bin,&bin);
        if (i-1>=0)
            SDL_GetRGBA(pixtab[j * width + i-1],format,&intensity2,&bin,&bin,&bin);
        break;

    case 135:
        if (j+1<height && i+1<height)
            SDL_GetRGBA(pixtab[(j+1) * width + i+1],format,&intensity1,&bin,&bin,&bin);
        if (j-1>=0 && i-1>=0)
            SDL_GetRGBA(pixtab[(j-1) * width + i-1],format,&intensity2,&bin,&bin,&bin);
        break;

    case 90:
        if (j+1<height)
            SDL_GetRGBA(pixtab[(j+1) * width + i],format,&intensity1,&bin,&bin,&bin);
        if (j-1>=0)
            SDL_GetRGBA(pixtab[(j-1) * width + i],format,&intensity2,&bin,&bin,&bin);
        break;

    case 45:
        if (i+1<width && j-1>=0)
            SDL_GetRGBA(pixtab[(j+1) * width + i-1],format,&intensity1,&bin,&bin,&bin);
        if (j+1<height && i-1>=0)
            SDL_GetRGBA(pixtab[(j-1) * width + i+1],format,&intensity2,&bin,&bin,&bin);
        break;

    default:
        break;  
    }

    if (Intensity_Gradian<intensity1 || Intensity_Gradian<intensity2)
    {
        Intensity_Gradian=0;
    }
    

    return SDL_MapRGBA(format, (Uint8)Intensity_Gradian, (Uint8)Intensity_Gradian, (Uint8)Intensity_Gradian, (Uint8)255);
}

SDL_Surface* Supp_Maxima(SDL_Surface* Intensity_Gradian_Image, SDL_Surface* Orientation_Gradian_Image)
{
    int height = Intensity_Gradian_Image->h;
    int width = Intensity_Gradian_Image->w;
    SDL_PixelFormat* format = Intensity_Gradian_Image->format;

    SDL_Surface *image_converted = SDL_CreateRGBSurfaceWithFormat(0, width, height, 32, format->format);
    Uint32* new_pixtab = image_converted->pixels;

    Uint32* pixtab_intensity = Intensity_Gradian_Image->pixels;
    Uint32* pixtab_orientation = Orientation_Gradian_Image->pixels;

    Uint8 intensity = 0, orientation = 0;
    Uint8 trash1,trash2,trash3;

    for (int y = 0; y < height; y++) 
    {
        for (int x = 0; x < width; x++) 
        {
            SDL_GetRGBA(pixtab_intensity[y * width + x], format, &intensity, &trash1, &trash2, &trash3);
            SDL_GetRGBA(pixtab_orientation[y * width + x], format, &orientation, &trash1, &trash2, &trash3);

            new_pixtab[y * width + x] = Sup_max_calculus(intensity, orientation, Intensity_Gradian_Image, x, y);
        }
    }

    return image_converted;
}

int Max_val_Neighbor(SDL_Surface* image, int x, int y)
{
    Uint8 max =0;

    int height = image->h;
    int width = image->w;
    const SDL_PixelFormat* format = image->format;
    Uint32* pixtab = image->pixels;
    Uint8 r,g,b,a;

    for (int j = y-1; j <= y+1; j++)
    {
        for (int i = x-1; i <= x+1; i++)
        {
            if (j>=0 && j<height && i>=0 && i<width)
            {
                SDL_GetRGBA(pixtab[j*width+i], format, &r, &g, &b, &a);
                if(r>max)
                    max = r;
            }
        }
    }

    return max;
}

void Thresholdhysteresis(SDL_Surface* image)
{
    int seuil_sup = Histogram_Seuil(image)/3;
    int seuil_min = seuil_sup * 1/2;

    int height = image->h;
    int width = image->w;
    const SDL_PixelFormat* format = image->format;
    Uint32* pixtab = image->pixels;

    Uint8 col;
    Uint8 trash;

    for (int y = 0; y < height; y++) 
    {
        for (int x = 0; x < width; x++) 
        {
            SDL_GetRGBA(pixtab[y * width + x], format, &col, &trash, &trash, &trash);

            if (col > seuil_sup)
                col = 255;
            else if (col < seuil_min)
                col = 0;
            else
                col = 127;

            pixtab[y * width + x] = SDL_MapRGBA(format, (Uint8)col, (Uint8)col, (Uint8)col, 255);
        }
    }

    for (int y = 0; y < height; y++) 
    {
        for (int x = 0; x < width; x++) 
        {
            SDL_GetRGBA(pixtab[y * width + x], format, &col, &trash, &trash, &trash);

            if (col == 127)
            {
                Uint8 max = Max_val_Neighbor(image, x, y);
                if (max == 255)
                    col = 255;
                else
                    col =0;
                pixtab[y * width + x] = SDL_MapRGBA(format, (Uint8)col, (Uint8)col, (Uint8)col, 255);
            }
            else
                continue;
        }
    }
}

//Calcul la somme des éléments d'un tableau de iind a indf
int SumTab(int tab[], int indd, int indf)
{
    int res = 0;
    for (int i = indd; i < indf; i++)
    {
        res += tab[i];
    }
    return res;
}

Uint8 Histogram_Seuil(SDL_Surface* image)
{
    int histo[255] = {0};

    int height = image->h;
    int width = image->w;
    const SDL_PixelFormat* format = image->format;

    Uint32* pixtab = image->pixels;

    Uint8 val;
    Uint8 trash;
    for (int y = 2; y < height-2; y++) 
    {
        for (int x = 2; x < width-2; x++) 
        {
           SDL_GetRGBA(pixtab[y*width+x],format, &val, &trash, &trash, &trash);
           histo[(int)val] =+1;
        }
    }
    
    Uint8 med = 1;

    while (SumTab(histo, 0 , med) != SumTab(histo, med+1, 255))
    {
        med ++;
    }
    
    return med;
}