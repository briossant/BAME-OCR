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

int LocalMaximum(size_t *accumu, size_t h_acc, size_t w_acc, int x, int y) {
  size_t max = 0;
  size_t last_max = 42;
  // TODO
  for (int j = y - 1; j <= y + 1; j++) {
    for (int i = x - 1; i <= x + 1; i++) {
      if (j >= 0 && j < h_acc && i >= 0 && i < w_acc) {
        size_t val = accumu[j * w_acc + i];
        if (val > max)
          max = val;
      }
    }
  }

  return max;
}

SDL_Surface *new_hough_transform(SDL_Surface *image, int delta, int threshold) {

  int height = image->h;
  int width = image->w;
  int h_acc = 2 * sqrt(height * height + width * width);
  int w_acc = 180 + delta;
  const SDL_PixelFormat *format = image->format;

  size_t *accumu = calloc(w_acc * h_acc, sizeof(size_t));

  Uint32 *pixtab = image->pixels;

  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {

      Uint8 r, g, b, a;
      SDL_GetRGBA(pixtab[y * width + x], format, &r, &g, &b, &a);
      if (r < 127)
        continue;

      for (double theta = 0; theta < w_acc; theta += 1) {
        int raw = (double)x * cos((double)theta / w_acc * M_PI) +
                  (double)y * sin((double)theta / w_acc * M_PI) +
                  (double)h_acc / 2;
        raw >>= 1;
        size_t coo = theta + raw * w_acc;
        accumu[coo]++;
      }
    }
  }

  size_t nb_droite = 0;
  for (int y = 0; y < h_acc; y++) {
    for (int x = 0; x < w_acc; x++) {
      if (accumu[y * w_acc + x] > threshold) {
        // search local max and set zone to 0
        // FindLocalMax
        ++nb_droite;
      }
    }
  }

  int *matrix = calloc(nb_droite * 4, sizeof(int));

  for (int y = 0; y < h_acc; y++) {
    for (int x = 0; x < w_acc; x++) {
      if (accumu[y * w_acc + x] > threshold) {
        double rho = y * 2 - (double)h_acc / 2,
               theta = (double)x * M_PI / w_acc;
        double A = cos(theta), B = sin(theta);
        double x0 = A * rho, y0 = B * rho;
        int x1 = (x0 + 10000 * (-B));
        int y1 = (y0 + 10000 * (A));
        int x2 = (x0 - 10000 * (-B));
        int y2 = (y0 - 10000 * (A));
        // draw_line(image, x1, y1, x2, y2);
      }
    }
  }
  return image;
}
