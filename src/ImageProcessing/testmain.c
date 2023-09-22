#include "ImageProcess.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>

int main(int argc, char *argv[])
{
    if (argc!= 3) 
    {
        errx(1, "Usage: %s <file to open> <file to save>\n", argv[0]);
    }

    if(0 != SDL_Init(SDL_INIT_VIDEO))
    {
        errx(1, "Unable to initialize SDL: %s\n", SDL_GetError());
    }

    SDL_Surface* image = NULL;
    image = IMG_Load(argv[1]);
    if (image == NULL) 
    {
        printf("Can't load image : %s\n", IMG_GetError());
        return 0;
    }

    SDL_PixelFormat* format = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888);
    SDL_Surface *image_converted = SDL_ConvertSurface(image, format, 0);
    SDL_Surface *image_converted1 = SDL_ConvertSurface(image, format, 0);
    SDL_FreeSurface(image);


    GreyScale(image_converted);
    //BlacknWhite(image_converted);
    //Contrast(image_converted);
    GaussianBlur(image_converted);
    //Bright(image_converted);
    //ArroundGaussianBlur(image_converted);
    //image_converted = Rotate(image_converted, 35);
    //image_converted = Canny(image_converted);
    image_converted = Intensity_Gradian(image_converted);
    image_converted1 = Orientation_Gradian(image_converted);
    image_converted = Supp_Maxima(image_converted, image_converted1);



    if (IMG_SavePNG(image_converted, argv[2]) != 0) 
    {
        printf("Error when trying to save the image : %s\n", IMG_GetError());
    }

    SDL_FreeSurface(image_converted);
    
    IMG_Quit();
    SDL_Quit();

    return 1;
}