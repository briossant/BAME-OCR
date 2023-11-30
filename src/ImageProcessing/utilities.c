
#include "ImageProcess.h"
// Input
SDL_Surface *SDL_Start(char *filename) {
  // SDL Init
  SDL_Surface *image = NULL;
  image = IMG_Load(filename);
  if (image == NULL) {
    printf("%s: cannot find %s: No such file or directory\n", filename,
           filename);
    // printf("Can't load image : %s\n", IMG_GetError());
    return NULL;
  }

  SDL_PixelFormat *format = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888);
  SDL_Surface *image_converted = SDL_ConvertSurface(image, format, 0);
  SDL_FreeSurface(image);
  return image_converted;
}

// Output
int SDL_Output(SDL_Surface *image_converted, char *file) {
  if (IMG_SavePNG(image_converted, file) != 0) {
    printf("Error when trying to save the image : %s\n", IMG_GetError());
  }

  SDL_FreeSurface(image_converted);

  IMG_Quit();
  SDL_Quit();

  return 1;
}
