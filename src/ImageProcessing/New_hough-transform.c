#include "ImageProcess.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_surface.h>
#include <err.h>
#include <math.h>
#include <stdlib.h>

SDL_Surface *new_hough_transform(SDL_Surface *image, int delta, int threshold) {

  int height = image->h;
  int width = image->w;
  int w_acc = 400;
  int h_acc = 400;
  const SDL_PixelFormat *format = image->format;

  SDL_Surface *accumu =
      SDL_CreateRGBSurfaceWithFormat(0, w_acc, h_acc, 32, format->format);

  Uint32 *pixtab = image->pixels;
  Uint32 *pixtab_acc = accumu->pixels;

  for (int y = 0; y < h_acc * w_acc; y++) {
    pixtab_acc[y] = SDL_MapRGBA(format, 0, 0, 0, 255);
  }
  double theta_max = M_PI / 2;
  double raw_max = sqrt(w_acc * w_acc + h_acc * h_acc);

  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {

      Uint8 r, g, b, a;
      SDL_GetRGBA(pixtab[y * width + x], format, &r, &g, &b, &a);
      if (r < 127)
        continue;

      double raw = sqrt(x * x + y * y);
      double theta = acos((double)x / raw);

      size_t coo = (theta / theta_max) * w_acc + raw / raw_max * h_acc;
      SDL_GetRGBA(pixtab[coo], format, &r, &g, &b, &a);
      pixtab_acc[coo] = SDL_MapRGBA(format, r + 1, g + 1, b + 1, a);
    }
  }
  return accumu;
}
