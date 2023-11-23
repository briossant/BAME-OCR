#include <SDL2/SDL.h>
#include <SDL2/SDL_surface.h>
#include <stdio.h>
#include <stdlib.h>

void BlacknWhite(SDL_Surface *image);

void GreyScale(SDL_Surface *image);

void Contrast(SDL_Surface *image);

void GaussianBlur(SDL_Surface *image);

void Bright(SDL_Surface *image);

void ArroundGaussianBlur(SDL_Surface *image);

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

SDL_Surface *hough_transform(SDL_Surface *image, int threshold, int state);

SDL_Surface *new_hough_transform(SDL_Surface *image, int delta, int threshold);

void draw_line(SDL_Surface *image, int x1, int y1, int x2, int y2);

void draw_hough_line(SDL_Surface *image, SDL_Surface *hough_pic, int seuil);

void Auto_Rotate(SDL_Surface *image);

void Crop(SDL_Surface *image, int x11, int y11, int x12, int y12, int x21,
          int y21, int x22, int y22, char *filename);
