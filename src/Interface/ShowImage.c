#include <SDL2/SDL.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_timer.h>
#include <stdlib.h>

int ShowImage (SDL_Texture* pic, SDL_Renderer* render)
{

    SDL_Rect rect = {0,0,640,480};

    if(SDL_QueryTexture(pic, NULL, NULL, &rect.w, &rect.h)!=0)
    {
        SDL_Log("Erreur > %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    if (SDL_RenderCopy(render, pic, NULL, &rect)!=0) 
    {
        SDL_Log("Erreur > %s\n", SDL_GetError());
        exit(EXIT_FAILURE);   
    }

    SDL_RenderPresent(render);
    SDL_Delay(5000);

    SDL_Quit();
    return EXIT_SUCCESS;
}
