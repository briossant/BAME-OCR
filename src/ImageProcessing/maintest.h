#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>

SDL_Texture *loadImage(const char path[], SDL_Renderer *renderer);

int ShowImage (SDL_Texture* pic, SDL_Renderer* render);
