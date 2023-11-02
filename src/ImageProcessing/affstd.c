#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_timer.h>
#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include "maintest.h"

int main()
{
    if(0 != SDL_Init(SDL_INIT_VIDEO))
        return EXIT_FAILURE;

    SDL_Window* win = NULL;
    SDL_Renderer* render =NULL;
    SDL_Texture* text = NULL;
    SDL_Surface* img = NULL;
    SDL_Rect rect = {0,0,1024,640};

    win = SDL_CreateWindow("ShowImage", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,1024, 640, SDL_WINDOW_SHOWN);
    if (win == NULL)
    {
        errx(1, "err: window");
    }


    render = SDL_CreateRenderer(win, -1, SDL_RENDERER_SOFTWARE);
    if (render == NULL)
    {
        errx(1, "err: render");
    }
    
    img = SDL_LoadBMP("image_test.bmp");
    if (img == NULL)
    {
        errx(1, "err: image load");
    }

    text = SDL_CreateTextureFromSurface(render, img);
    if (text == NULL)
    {
        errx(1, "err: texture");
    }

    if (SDL_QueryTexture(text, NULL, NULL, &rect.w, &rect.h) != 0)
    {
        errx(1,"err: QueryTexture");
    }   

    if (SDL_RenderCopy(render, text, NULL, &rect) != 0)
    {
        errx(1,"err: RenderCopy");
    }

    SDL_RenderPresent(render);
    SDL_Delay(5000);
    SDL_Quit();
    return 1;
}
