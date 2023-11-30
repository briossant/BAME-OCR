#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_surface.h>
#include <err.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  int x;
  int y;
} pair;

void BlacknWhite(SDL_Surface *image);

void GreyScale(SDL_Surface *image);

void Contrast(SDL_Surface *image);

void GaussianBlur(SDL_Surface *image);

void Bright(SDL_Surface *image);

void ArroundGaussianBlur(SDL_Surface *image);

SDL_Surface *SDL_Start(char *filename);

int SDL_Output(SDL_Surface *image_converted, char *file);

SDL_Surface *Rotate(SDL_Surface *image, double angle);

SDL_Surface *Canny(SDL_Surface *image);

SDL_Surface *Intensity_Gradian(SDL_Surface *image);

SDL_Surface *Orientation_Gradian(SDL_Surface *image);

Uint32 convolution(SDL_Surface *image, int x, int y, int *kernel,
                   int kernelWidth);

SDL_Surface *Supp_Maxima(SDL_Surface *Intensity_Gradian_Image,
                         SDL_Surface *Orientation_Gradian_Image);

void Thresholdhysteresis(SDL_Surface *image);

Uint8 ComputeSeuil(SDL_Surface *image);

int *hough_transform(SDL_Surface *image, int *return_size);

void draw_line(SDL_Surface *image, int x1, int y1, int x2, int y2,
               Uint32 color);

void Auto_Rotate(SDL_Surface *image);

SDL_Surface *Crop(SDL_Surface *image, pair p1, pair p2);

void Crop_grid(SDL_Surface *image, int *lx, int *ly);

SDL_Surface *Resize_crop(SDL_Surface *image, int x1, int y1, int x2, int y2);

void InvertColor(SDL_Surface *image);

void Balance(SDL_Surface *image);

int *GridDetection(int *lines, int nbr_of_lines);
