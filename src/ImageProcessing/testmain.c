#include "ImageProcess.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>

int main(int argc, char *argv[])
{
    if (argc!= 2) 
    {
        errx(1, "Usage: %s <file>\n", argv[0]);
    }

    if(0 != SDL_Init(SDL_INIT_VIDEO))
    {
        errx(1, "Unable to initialize SDL: %s\n", SDL_GetError());
    }


    GreyScale(argv[1]);

    SDL_Quit();
    return 0;
}