#include "ImageProcess.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

void GreyScale(char *path)
{
    if (IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) == 0) 
    {
    printf("Error when initializing SDL2_image : %d\n", IMG_GetError());
    }
    
    struct SDL_Surface* image = IMG_Load(path);
    if (!image) 
    {
    printf("Can't load image : %d\n", IMG_GetError());
    }


}
