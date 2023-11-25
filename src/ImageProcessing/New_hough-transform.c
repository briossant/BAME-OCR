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

SDL_Surface *new_hough_transform(SDL_Surface *image, int delta, int threshold) {

  int height = image->h;
  int width = image->w;
  int w_acc = 1000;
  int h_acc = 2 * sqrt(height * height + width * width);
  int step = delta;
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

      for (int theta = 0; theta < w_acc * step; theta += 1) {
        int raw = (double)x * cos((double)theta / step / w_acc * M_PI) +
                  (double)y * sin((double)theta / step / w_acc * M_PI) +
                  h_acc / 2;
        raw >>= 1;
        // if (raw == 0)
        // raw = (double)y * cos(theta) + (double)x * sin(theta);
        size_t coo = theta / step + raw * w_acc;
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
      SDL_GetRGBA(pixtab_acc[y * w_acc + x], format, &r, &g, &b, &a);
      if (g > threshold) {
        double rho = x * 2 - h_acc / 2, theta = (double)y * M_PI / w_acc;
        double a = cos(theta), b = sin(theta);
        double x0 = a * rho, y0 = b * rho;
        int x1 = (x0 + 1000 * (-b));
        int y1 = (y0 + 1000 * (a));
        int x2 = (x0 - 1000 * (-b));
        int y2 = (y0 - 1000 * (a));
        printf("%d %d -- %d %d %d \n", x1, y1, x, y, r);
        draw_line(image, x1, y1, x2, y2);
      }
    }
  }
  IMG_SavePNG(image, "line.png");
  return accumu;
}
