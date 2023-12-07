#include "ImageProcess.h"

SDL_Texture *loadImage(const char path[], SDL_Renderer *renderer) {
  SDL_Surface *tmp = NULL;
  SDL_Texture *texture = NULL;
  tmp = SDL_LoadBMP(path);
  if (NULL == tmp) {
    fprintf(stderr, "Erreur SDL_LoadBMP : %s", SDL_GetError());
    return NULL;
  }
  texture = SDL_CreateTextureFromSurface(renderer, tmp);
  SDL_FreeSurface(tmp);
  if (NULL == texture) {
    fprintf(stderr, "Erreur SDL_CreateTextureFromSurface : %s", SDL_GetError());
    return NULL;
  }
  return texture;
}
