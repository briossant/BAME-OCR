

#include "ImageProcess.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_surface.h>
#include <err.h>
#include <math.h>
#include <stdlib.h>

#define TRESHOLD 62
#define MIN_DIST 22
#define diagonal_killer 4

int IsSimilar(int a, int b) { return abs(a - b) <= TRESHOLD; }

// recursive function
int CheckIfDistIsValid(int *points, size_t len, int dist, int i, int count) {
  if (count == 10)
    return points[i];
  for (size_t j = i + 1; j < len; ++j) {
    if (IsSimilar(abs(points[j] - points[i]), dist))
      return CheckIfDistIsValid(points, len, dist, j, count + 1);
  }
  return -1;
}

int *GridDetectionAux(int *points, size_t len) {
  for (size_t i = 0; i < len; i++) {
    for (size_t j = i + 1; j < len; j++) {
      int dist = abs(points[j] - points[i]);
      if (dist < MIN_DIST) {
        continue;
      }
      int k;
      if ((k = CheckIfDistIsValid(points, len, dist, j, 1)) != -1) {
        int *res = malloc(sizeof(int) * 2);
        res[0] = points[i];
        res[1] = k;
        return res;
      }
    }
  }
  errx(0, "unabled to find a grid");
}
// main function
int *GridDetection(int *lines, int nbr_of_lines) {
  int *vertical = malloc(sizeof(int) * nbr_of_lines);
  int *horizontal = malloc(sizeof(int) * nbr_of_lines);
  size_t v_i = 0;
  size_t h_i = 0;

  for (int i = 0; i < nbr_of_lines * 4; i += 4) {
    // Todo: Check if lines are not very straight (diagonal case)
    if (abs(lines[i] - lines[i + 2]) >
        abs(lines[i + 1] - lines[i + 3]) * diagonal_killer) {
      // Horizontal
      horizontal[h_i++] = (lines[i + 1] + lines[i + 3]) / 2;
      printf("h: %d \n", horizontal[h_i - 1]);
    } else if (abs(lines[i] - lines[i + 2]) * diagonal_killer <
               abs(lines[i + 1] - lines[i + 3])) {

      // Vertical
      vertical[v_i++] = (lines[i] + lines[i + 2]) / 2;
      // printf("v: %d \n", vertical[v_i - 1]);
    }
  }

  printf("v: %ld , h: %ld \n", v_i, h_i);
  int *y = GridDetectionAux(horizontal, h_i);
  printf("y: %d %d\n", y[0], y[1]);
  int *x = GridDetectionAux(vertical, v_i);

  printf("x: %d %d\n", x[0], x[1]);
  int *matrix = malloc(sizeof(int) * 4);
  matrix[0] = x[0];
  matrix[1] = y[0];
  matrix[2] = x[1];
  matrix[3] = y[1];

  return matrix;
}
