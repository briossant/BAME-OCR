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


int void_square(SDL_Surface *image)
{
  int accu = 0;
  int height = image->h;
  int width = image->w;
  const SDL_PixelFormat* format = image->format;
  Uint32* pixtab = image->pixels;

  for (int x = 0; x < height*width; x++)
  {
    Uint8 r, g, b, a;
    SDL_GetRGBA(pixtab[x],format, &r, &g, &b, &a);
    if (!r)
      accu++;

  }
  accu = accu/(height*width);
  return accu < 0.25;
}

SDL_Surface* Resize_crop(SDL_Surface* image, int x1, int y1, int x2, int y2)
{

  int height = image->h;
  int width = image->w;

  if(x2>width || x1>width){
    err(0,"Cordinates of x1 or x2 must be in bounds");
  }
  if(y2>height || y1>height){
    err(0,"Cordinates of y1 or y2 must be in bounds");
  }

  const SDL_PixelFormat* format = image->format;

  SDL_Surface *image_converted = SDL_CreateRGBSurfaceWithFormat(0, 32
            , 32, 32, format->format);  
  
  const SDL_Rect src = {.x=x1,.y=y1,.w=x2-x1,.h=y2-y1};

  int er = SDL_BlitScaled(image, &src, image_converted,NULL);
  if (er == -1)
    err(0,NULL);



  SDL_Surface *image_converted2 = SDL_CreateRGBSurfaceWithFormat(0, 28
            , 28, 32, format->format);  
  
  const SDL_Rect src2 = {.x=2,.y=2,.w=28,.h=28};

  er = SDL_BlitScaled(image_converted, &src2, image_converted2,NULL);
  if (er == -1)
    err(0,NULL);

  SDL_FreeSurface(image_converted);  

  // printf("Is empty? = %i\n",void_square(image_converted));
  return image_converted2;
}

/*#define PATCH_W 36
#define PATCH_H 36

void LocalMaximum(int *accumu, int h_acc, int w_acc, int x, int y) {
  int max_coo = y * w_acc + x;
  int max = accumu[max_coo];
  int w_half = PATCH_W / 2;
  int coo;
  accumu[max_coo] = -accumu[max_coo];

  for (int j = y; j <= y + PATCH_H && j < h_acc; j++) {
    for (int i = x - w_half; i >= 0 && i <= x + w_half && i < w_acc; i++) {
      coo = j * w_acc + i;
      if (accumu[coo] > max) {
        max = accumu[coo];
        accumu[max_coo] = 0;
        max_coo = coo;
        accumu[max_coo] = -accumu[max_coo];
      } else
        accumu[coo] = 0;
    }
  }
}*/

void LocalMaximum(int *accumu, int h_acc, int w_acc, int coo, int threshold) {
  size_t size = 24200;
  int *queue = malloc(sizeof(int) * size);
  size_t i_current = 0;
  size_t i_insert = 1;
  queue[0] = coo;
  int max_coo = coo;

  // check for buffer overflow
  while (i_current < i_insert) {
    int x = queue[i_current] % w_acc;
    int y = queue[i_current] / w_acc;
    coo = queue[i_current];
    ++i_current; // dequeue

    if (x + 1 < w_acc) {
      if (accumu[coo + 1] > threshold)
        queue[i_insert++] = coo + 1;
      if (accumu[coo + 1] > accumu[max_coo]) {
        accumu[max_coo] = 0;
        max_coo = coo + 1;
      } else
        accumu[coo + 1] = 0;
    }

    if (x - 1 >= 0) {
      if (accumu[coo - 1] > threshold)
        queue[i_insert++] = coo - 1;
      if (accumu[coo - 1] > accumu[max_coo]) {
        accumu[max_coo] = 0;
        max_coo = coo - 1;
      } else
        accumu[coo - 1] = 0;
    }

    if (y + w_acc < h_acc) {
      if (accumu[coo + w_acc] > threshold)
        queue[i_insert++] = coo + w_acc;
      if (accumu[coo + w_acc] > accumu[max_coo]) {
        accumu[max_coo] = 0;
        max_coo = coo + w_acc;
      } else
        accumu[coo + w_acc] = 0;
    }
    /* useless bitch
        if (y - w_acc >= 0) {
          if (accumu[coo - w_acc] > threshold)
            queue[i_insert++] = coo - w_acc;
          if (accumu[coo - w_acc] > accumu[max_coo]) {
            accumu[max_coo] = 0;
            max_coo = coo - w_acc;
          } else
            accumu[coo - w_acc] = 0;
        }*/
  }
  accumu[max_coo] = -accumu[max_coo];
  free(queue);
}

int *new_hough_transform(SDL_Surface *image, int delta, int threshold,
                         int *return_size) {

  int height = image->h;
  int width = image->w;
  int h_acc = 2 * sqrt(height * height + width * width);
  int w_acc = 180 + delta;
  const SDL_PixelFormat *format = image->format;

  int *accumu = calloc(w_acc * h_acc, sizeof(size_t));

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

  int max = accumu[0];
  for (int y = 0; y < h_acc * w_acc; y++) {
    if (accumu[y] > max) {
      max = accumu[y];
    }
  }
  threshold = max / 3.5;

  size_t nb_droite = 0;
  for (int y = 0; y < h_acc * w_acc; y++) {
    if (accumu[y] > threshold) {
      LocalMaximum(accumu, h_acc, w_acc, y, threshold);
      ++nb_droite;
    }
  }

  int *matrix = calloc(nb_droite * 4, sizeof(int));
  size_t i = 0;
  for (int y = 0; y < h_acc; y++) {
    for (int x = 0; x < w_acc; x++) {
      if (accumu[y * w_acc + x] < -threshold) {
        double rho = y * 2 - (double)h_acc / 2,
               theta = (double)x * M_PI / w_acc;
        double A = cos(theta), B = sin(theta);
        double x0 = A * rho, y0 = B * rho;
        matrix[i++] = (x0 + 10000 * (-B));
        matrix[i++] = (y0 + 10000 * (A));
        matrix[i++] = (x0 - 10000 * (-B));
        matrix[i++] = (y0 - 10000 * (A));
        draw_line(image, matrix[i - 4], matrix[i - 3], matrix[i - 2],
                  matrix[i - 1]);
        // test of Grid Detection
      }
    }
  }
  *return_size = nb_droite;
  return matrix;
}
