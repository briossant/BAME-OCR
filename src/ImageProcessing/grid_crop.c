/*
Number of try to write Hough : 3
*/

#include "ImageProcess.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_surface.h>
#include <err.h>
#include <math.h>
#include <stdlib.h>

int void_square(SDL_Surface *image) {
  int accu = 0;
  int height = image->h;
  int width = image->w;
  const SDL_PixelFormat *format = image->format;
  Uint32 *pixtab = image->pixels;

  for (int x = 0; x < height * width; x++) {
    Uint8 r, g, b, a;
    SDL_GetRGBA(pixtab[x], format, &r, &g, &b, &a);
    if (!r)
      accu++;
  }
  accu = accu / (height * width);
  return accu < 0.25;
}

SDL_Surface *Resize_crop(SDL_Surface *image, int x1, int y1, int x2, int y2) {

  int height = image->h;
  int width = image->w;

  if (x2 > width || x1 > width) {
    err(0, "Cordinates of x1 or x2 must be in bounds");
  }
  if (y2 > height || y1 > height) {
    err(0, "Cordinates of y1 or y2 must be in bounds");
  }

  const SDL_PixelFormat *format = image->format;

  SDL_Surface *image_converted =
      SDL_CreateRGBSurfaceWithFormat(0, 32, 32, 32, format->format);

  const SDL_Rect src = {.x = x1, .y = y1, .w = x2 - x1, .h = y2 - y1};

  int er = SDL_BlitScaled(image, &src, image_converted, NULL);
  if (er == -1)
    err(0, NULL);

  SDL_Surface *image_converted2 =
      SDL_CreateRGBSurfaceWithFormat(0, 28, 28, 32, format->format);

  const SDL_Rect src2 = {.x = 2, .y = 2, .w = 28, .h = 28};

  er = SDL_BlitScaled(image_converted, &src2, image_converted2, NULL);
  if (er == -1)
    err(0, NULL);

  SDL_FreeSurface(image_converted);

  // printf("Is empty? = %i\n",void_square(image_converted));
  return image_converted2;
}
