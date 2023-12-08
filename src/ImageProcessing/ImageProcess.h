#define _GNU_SOURCE

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#pragma GCC diagnostic ignored "-Wtype-limits"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_surface.h>
#include <err.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#pragma GCC diagnostic pop

//////// DEFINES /////////
// global
#define IMAGE_SIZE 2800

// grid detection
#define GD_TRESHOLD 142.42
#define GD_MIN_DIST 52 // set min from image size
#define GD_diagonal_killer 6

// Balance
#define THRESHOLD_BALANCE 2
#define BALANCE_MIN_DIFF 42

#define THRESHOLD_VOID_SQUARE 42
#define THRESHOLD_VOID_SQUARE_2 202

// hough transform
#define HOUGH_THRESHOLD 0.42

// Crop and number centering
#define FIRST_CROP_SIZE 58
#define NN_IMAGE_SIZE 28 // mnist size
#define TRESHOLD_NUMBER_DETECTION 24
#define OFFSET_CENTER_NUMBER 2
#define OFFSET_FIRST_CROP 28
//////// DEFINES END //////

typedef struct {
    int x;
    int y;
} pair;

void SortList(double *list, size_t len);

int mostBrightPixel(SDL_Surface *image);

int lessBrightPixel(SDL_Surface *image);

double GetImageAngleAndRotateHoughMatrix(double *matrix_hough, size_t len);

void BlacknWhite(SDL_Surface *image);

void GreyScale(SDL_Surface *image);

SDL_Surface *StandardizeImage(SDL_Surface *image);

void ComputeStardizeLens(int width, int height, int *dx, int *dy);

void Contrast(SDL_Surface *image);

SDL_Surface *bilateralFilterOwn(SDL_Surface *image, int diameter, double sigmaI,
                                double sigmaS);

void GaussianBlur(SDL_Surface *image);

void Bright(SDL_Surface *image);

void Balance(SDL_Surface *image);

void ArroundGaussianBlur(SDL_Surface *image);

SDL_Surface *SDL_Start(char *filename);

int SDL_Output(SDL_Surface *image_converted, char *file);

SDL_Surface *Rotate(SDL_Surface *image, double angle);

SDL_Surface *Canny(SDL_Surface *image, int old_width, int old_height);

SDL_Surface *Intensity_Gradian(SDL_Surface *image, int old_width,
                               int old_height);

SDL_Surface *Orientation_Gradian(SDL_Surface *image);

Uint32 convolution(SDL_Surface *image, int x, int y, int *kernel,
                   int kernelWidth);

SDL_Surface *Supp_Maxima(SDL_Surface *Intensity_Gradian_Image,
                         SDL_Surface *Orientation_Gradian_Image);

void Thresholdhysteresis(SDL_Surface *image);

Uint8 ComputeSeuil(SDL_Surface *image);

double *hough_transform(SDL_Surface *image, int *return_size);

int *TransformHoughPolarToPoints(double *hough_matrix, size_t len);

void draw_line(SDL_Surface *image, int x1, int y1, int x2, int y2,
               Uint32 color);

void Auto_Rotate(SDL_Surface *image);

SDL_Surface *Crop(SDL_Surface *image, pair p1, pair p2);

void Crop_grid(SDL_Surface *image, int *lx, int *ly);

SDL_Surface *Resize_crop(SDL_Surface *image, int x1, int y1, int x2, int y2);

void InvertColor(SDL_Surface *image);

int void_square(SDL_Surface *image);

int *GridDetection(int *lines, int nbr_of_lines);

SDL_Surface *CenterNumber(SDL_Surface *image);

void get_green_number(SDL_Surface *image, int number, int start, int end,
                      char *path);
