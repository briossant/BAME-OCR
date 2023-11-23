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
  int w_acc = 1000;
  int h_acc = 2 * sqrt(height * height + width * width);
  double step = 0.001;
  const SDL_PixelFormat *format = image->format;

  SDL_Surface *accumu =
      SDL_CreateRGBSurfaceWithFormat(0, w_acc, h_acc, 32, format->format);

  Uint32 *pixtab = image->pixels;
  Uint32 *pixtab_acc = accumu->pixels;

  for (int y = 0; y < h_acc * w_acc; y++) {
    pixtab_acc[y] = SDL_MapRGBA(format, 0, 0, 0, 255);
  }

  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {

      Uint8 r, g, b, a;
      SDL_GetRGBA(pixtab[y * width + x], format, &r, &g, &b, &a);
      if (r < 127)
        continue;

      for (double theta = 0; theta < M_PI; theta += step) {
        int raw = (double)x * cos(theta) + (double)y * sin(theta) + h_acc / 2;
        raw >>= 1;
        // if (raw == 0)
        // raw = (double)y * cos(theta) + (double)x * sin(theta);
        size_t coo = (int)(theta / M_PI * w_acc) + raw * w_acc;
        SDL_GetRGBA(pixtab_acc[coo], format, &r, &g, &b, &a);
        if (r >= 255) {
          continue;
        }
        pixtab_acc[coo] = SDL_MapRGBA(format, r + 1, g + 1, b + 1, a);
      }
    }
  }

  for (int y = 0; y < w_acc; y++) {
    for (int x = 0; x < h_acc; x++) {
      Uint8 r, g, b, a;
      SDL_GetRGBA(pixtab_acc[y * width + x], format, &r, &g, &b, &a);
      if (r > threshold) {
        // dessiner droite
      }
    }
  }

  return accumu;
}
