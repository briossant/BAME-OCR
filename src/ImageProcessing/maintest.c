#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include "maintest.h"

int main()
{
    if(0 != SDL_Init(SDL_INIT_VIDEO))
        return EXIT_FAILURE;

    SDL_Texture* tmp =NULL;
    SDL_Renderer* render =NULL;
    SDL_Window* win = NULL;
    win = SDL_CreateWindow("SDL2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1024, 640, 0);
    render = SDL_CreateRenderer(win, -1,0);
    /*on charge et affiche l'image. Attention seul les .bmp sont supporté*/

    tmp = loadImage("image_test.bmp", render);
    ShowImage(tmp,render);

    SDL_Delay(5000);

    SDL_Quit();
    return 1;
}
