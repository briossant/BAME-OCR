#include "ImageProcess.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <complex.h>
#include <stdio.h>
#include <stdlib.h>

void BlacknWhite(SDL_Surface *image) {
  Uint8 biais = 127;

  int height = image->h;
  int width = image->w;
  const SDL_PixelFormat *format = image->format;
  Uint32 *pixtab = image->pixels;
  Uint8 r, g, b, a;

  for (int x = 0; x < width * height; x++) {
    SDL_GetRGBA(pixtab[x], format, &r, &g, &b, &a);

    if ((r + g + b) / 3 > biais) {
      r = 255;
      g = 255;
      b = 255;
    } else {
      r = 0;
      g = 0;
      b = 0;
    }

    // Pixel Update
    pixtab[x] = SDL_MapRGBA(format, r, g, b, a);
  }
}

void InvertColor(SDL_Surface *image) {
  int height = image->h;
  int width = image->w;
  const SDL_PixelFormat *format = image->format;
  Uint32 *pixtab = image->pixels;
  Uint8 r, g, b, a;

  for (int x = 0; x < height * width; x++) {
    SDL_GetRGBA(pixtab[x], format, &r, &g, &b, &a);

    r = 255 - r;
    g = 255 - g;
    b = 255 - b;

    // Pixel Update
    pixtab[x] = SDL_MapRGBA(format, r, g, b, a);
  }
}

void Balance(SDL_Surface *image) {
  // int min = 255;
  // int max = 0;
  // size_t count = 0;

  int height = image->h;
  int width = image->w;
  const SDL_PixelFormat *format = image->format;
  Uint32 *pixtab = image->pixels;
  Uint8 r, g, b, a;

  SDL_Surface *image_converted = SDL_ConvertSurface(image, format, 0);
  Bright(image_converted);
  GreyScale(image);

  int count_white = 0;
  int invert = 0;

  // int ker[] = {1, 1, 1, 1, 1, 1, 1, 1, 1};

  for (int x = 0; x < height * width; x++) {
    SDL_GetRGBA(pixtab[x], format, &r, &g, &b, &a);
    // convolution(image, x % width, x / width, ker, 3);
    if (r > 50) {
      count_white++;
    }
    /*
    if (r > max)
      max = r;
    if (r < min)
      min = r;
    count += r;
    */
  }
  /*
      int middle = (max + min) / 3;
      middle += count / (width * height);
      middle /= 2;
  */

  Uint8 threshold = ComputeSeuil(image_converted) / 4;
  printf("%i\n", ComputeSeuil(image_converted) / 4);
  if (count_white >
      height * width - count_white) // Black caracter on white background
    invert = 1;

  size_t size = 800;
  int *queue = malloc(sizeof(int) * size);
  size_t i_current = 0;
  size_t i_insert = 1;
  queue[0] = 0;
  int coo = 0;

  while (i_current < i_insert) {
    int x = queue[i_current] % width;
    int y = queue[i_current] / width;
    coo = queue[i_current];
    ++i_current; // dequeue
    Uint8 r, g, b, a, r2, g2, b2, a2;
    SDL_GetRGBA(pixtab[coo], format, &r, &g, &b, &a);

    if (x + 1 < width) {
      SDL_GetRGBA(pixtab[coo + 1], format, &r2, &g2, &b2, &a2);
      if (r - r2 <= threshold && a2 != 254) {
        queue[i_insert++] = coo + 1;
        pixtab[coo + 1] = SDL_MapRGBA(format, r2, g2, b2, 254);
      }
    }

    if (x - 1 >= 0) {
      SDL_GetRGBA(pixtab[coo - 1], format, &r2, &g2, &b2, &a2);
      if (r - r2 <= threshold && a2 != 254) {
        queue[i_insert++] = coo - 1;
        pixtab[coo - 1] = SDL_MapRGBA(format, r2, g2, b2, 254);
      }
    }

    if (y + 1 < height) {
      SDL_GetRGBA(pixtab[coo + width], format, &r2, &g2, &b2, &a2);
      if (r - r2 <= threshold && a2 != 254) {
        queue[i_insert++] = coo + width;
        pixtab[coo + width] = SDL_MapRGBA(format, r2, g2, b2, 254);
      }
    }
    pixtab[coo] = SDL_MapRGBA(format, 255, 255, 255, 254);
  }
  free(queue);

  /*
  for (int x = 0; x < height * width; x++) {
    SDL_GetRGBA(pixtab[x], format, &r, &g, &b, &a);

    if (invert && r > middle) {
      r = 255;
      g = 255;
      b = 255;
    } else if (!invert && r <= middle) {
      r = 0;
      g = 0;
      b = 0;
    }

    // Pixel Update
    pixtab[x] = SDL_MapRGBA(format, r, g, b, a);
  }
*/
  if (invert)
    InvertColor(image);
}
