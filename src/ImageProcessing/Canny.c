#include "ImageProcess.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_surface.h>
#include <err.h>
#include <math.h>
#include <stdlib.h>

SDL_Surface *Canny(SDL_Surface *image, int old_width, int old_height) {
    GreyScale(image);
    GaussianBlur(image);

    SDL_PixelFormat *format = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888);
    SDL_Surface *image_converted = SDL_ConvertSurface(image, format, 0);
    SDL_Surface *image_converted1 = SDL_ConvertSurface(image, format, 0);
    SDL_FreeSurface(image);

    image_converted = Intensity_Gradian(image_converted, old_width, old_height);
    image_converted1 = Orientation_Gradian(image_converted);
    image_converted = Supp_Maxima(image_converted, image_converted1);
    Thresholdhysteresis(image_converted);
    SDL_FreeSurface(image_converted1);

    return image_converted;
}

int convolution_grayscale(SDL_Surface *image, int x, int y, int *kernel,
                          int kernelWidth) {
    int sum_coef = 0;
    int sum_r = 0;

    int height = image->h;
    int width = image->w;
    const SDL_PixelFormat *format = image->format;
    Uint32 *pixtab = image->pixels;
    Uint8 r, g, b, a;

    for (int j = y - (kernelWidth / 2); j <= y + (kernelWidth / 2); j++) {
        for (int i = x - (kernelWidth / 2); i <= x + (kernelWidth / 2); i++) {
            sum_coef += kernel[(j - (y - (kernelWidth / 2))) * kernelWidth +
                               (i - (x - (kernelWidth / 2)))];
            int coef_act = kernel[(j - (y - (kernelWidth / 2))) * kernelWidth +
                                  (i - (x - (kernelWidth / 2)))];
            if (j >= 0 && j < height && i >= 0 && i < width) {
                SDL_GetRGBA(pixtab[j * width + i], format, &r, &g, &b, &a);
                sum_r += coef_act * (int)r;
            }
        }
    }
    if (sum_coef != 0)
        sum_r /= sum_coef;

    return sum_r;
}

SDL_Surface *Intensity_Gradian(SDL_Surface *image, int old_width,
                               int old_height) {
    int height = image->h;
    int width = image->w;
    const SDL_PixelFormat *format = image->format;

    SDL_Surface *image_converted =
        SDL_CreateRGBSurfaceWithFormat(0, width, height, 32, format->format);

    Uint32 *new_pixtab = image_converted->pixels;

    int grad_x = 0;
    int grad_y = 0;
    int grad = 0;

    int grad_x_kernel[] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};

    int grad_y_kernel[] = {-1, -2, -1, 0, 0, 0, 1, 2, 1};

    // limit can be ajust for more or less precision

    for (int y = 2; y < old_height - 2; y++) {
        for (int x = 2; x < old_width - 2; x++) {
            grad_x = convolution_grayscale(image, x, y, grad_x_kernel, 3) / 4;
            grad_y = convolution_grayscale(image, x, y, grad_y_kernel, 3) / 4;

            grad = sqrt(grad_x * grad_x + grad_y * grad_y);

            new_pixtab[y * width + x] =
                SDL_MapRGBA(format, (Uint8)grad, (Uint8)grad, (Uint8)grad, 255);
        }
    }

    // SDL_FreeSurface(image);

    return image_converted;
}

SDL_Surface *Orientation_Gradian(SDL_Surface *image) {
    int height = image->h;
    int width = image->w;
    const SDL_PixelFormat *format = image->format;

    SDL_Surface *image_converted =
        SDL_CreateRGBSurfaceWithFormat(0, width, height, 32, format->format);

    Uint32 *new_pixtab = image_converted->pixels;

    int grad_x = 0;
    int grad_y = 0;
    int grad = 0;

    int grad_x_kernel[] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};

    int grad_y_kernel[] = {-1, -2, -1, 0, 0, 0, 1, 2, 1};

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            grad_x = convolution_grayscale(image, x, y, grad_x_kernel, 3);
            grad_y = convolution_grayscale(image, x, y, grad_y_kernel, 3);

            grad = atan2(grad_x, grad_y);

            new_pixtab[y * width + x] =
                SDL_MapRGBA(format, (Uint8)grad, (Uint8)grad, (Uint8)grad, 255);
        }
    }
    // need change modif in place
    // SDL_FreeSurface(image);
    return image_converted;
}

int roundToNearestAngle(int angle) {
    int roundedAngle;

    if (angle >= -22.5 && angle < 22.5) {
        roundedAngle = 0;
    } else if (angle >= 22.5 && angle < 67.5) {
        roundedAngle = 45;
    } else if (angle >= 67.5 && angle < 112.5) {
        roundedAngle = 90;
    } else if (angle >= 112.5 && angle < 157.5) {
        roundedAngle = 135;
    } else if (angle >= 157.5 || angle < -157.5) {
        roundedAngle = 0;
    } else if (angle >= -157.5 && angle < -112.5) {
        roundedAngle = 45;
    } else if (angle >= -112.5 && angle < -67.5) {
        roundedAngle = 90;
    } else {
        roundedAngle = 135;
    }

    return roundedAngle;
}

static Uint32 Sup_max_calculus(Uint8 Intensity_Gradian,
                               Uint8 Orientation_Gradian, SDL_Surface *surface,
                               int i, int j) {
    Uint32 *pixtab = surface->pixels;
    int height = surface->h;
    int width = surface->w;
    SDL_PixelFormat *format = surface->format;

    int angle = roundToNearestAngle((int)Orientation_Gradian);

    Uint8 bin, intensity1 = 0, intensity2 = 0;

    switch (angle) {
    case 0:
        if (i + 1 < width)
            SDL_GetRGBA(pixtab[j * width + i + 1], format, &intensity1, &bin,
                        &bin, &bin);
        if (i - 1 >= 0)
            SDL_GetRGBA(pixtab[j * width + i - 1], format, &intensity2, &bin,
                        &bin, &bin);
        break;

    case 135:
        if (j + 1 < height && i + 1 < height)
            SDL_GetRGBA(pixtab[(j + 1) * width + i + 1], format, &intensity1,
                        &bin, &bin, &bin);
        if (j - 1 >= 0 && i - 1 >= 0)
            SDL_GetRGBA(pixtab[(j - 1) * width + i - 1], format, &intensity2,
                        &bin, &bin, &bin);
        break;

    case 90:
        if (j + 1 < height)
            SDL_GetRGBA(pixtab[(j + 1) * width + i], format, &intensity1, &bin,
                        &bin, &bin);
        if (j - 1 >= 0)
            SDL_GetRGBA(pixtab[(j - 1) * width + i], format, &intensity2, &bin,
                        &bin, &bin);
        break;

    case 45:
        if (i + 1 < width && j - 1 >= 0)
            SDL_GetRGBA(pixtab[(j + 1) * width + i - 1], format, &intensity1,
                        &bin, &bin, &bin);
        if (j + 1 < height && i - 1 >= 0)
            SDL_GetRGBA(pixtab[(j - 1) * width + i + 1], format, &intensity2,
                        &bin, &bin, &bin);
        break;

    default:
        break;
    }

    if (Intensity_Gradian < intensity1 || Intensity_Gradian < intensity2) {
        Intensity_Gradian = 0;
    }

    return SDL_MapRGBA(format, (Uint8)Intensity_Gradian,
                       (Uint8)Intensity_Gradian, (Uint8)Intensity_Gradian,
                       (Uint8)255);
}

SDL_Surface *Supp_Maxima(SDL_Surface *Intensity_Gradian_Image,
                         SDL_Surface *Orientation_Gradian_Image) {
    int height = Intensity_Gradian_Image->h;
    int width = Intensity_Gradian_Image->w;
    SDL_PixelFormat *format = Intensity_Gradian_Image->format;

    SDL_Surface *image_converted =
        SDL_CreateRGBSurfaceWithFormat(0, width, height, 32, format->format);

    Uint32 *new_pixtab = image_converted->pixels;

    Uint32 *pixtab_intensity = Intensity_Gradian_Image->pixels;
    Uint32 *pixtab_orientation = Orientation_Gradian_Image->pixels;

    Uint8 intensity = 0, orientation = 0;
    Uint8 trash1, trash2, trash3;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            SDL_GetRGBA(pixtab_intensity[y * width + x], format, &intensity,
                        &trash1, &trash2, &trash3);
            SDL_GetRGBA(pixtab_orientation[y * width + x], format, &orientation,
                        &trash1, &trash2, &trash3);

            new_pixtab[y * width + x] = Sup_max_calculus(
                intensity, orientation, Intensity_Gradian_Image, x, y);
        }
    }

    // SDL_FreeSurface(Intensity_Gradian_Image);
    // SDL_FreeSurface(Orientation_Gradian_Image);
    return image_converted;
}

int Max_val_Neighbor(SDL_Surface *image, int x, int y) {
    Uint8 max = 0;

    int height = image->h;
    int width = image->w;
    const SDL_PixelFormat *format = image->format;
    Uint32 *pixtab = image->pixels;
    Uint8 r, g, b, a;

    for (int j = y - 1; j <= y + 1; j++) {
        for (int i = x - 1; i <= x + 1; i++) {
            if (j >= 0 && j < height && i >= 0 && i < width) {
                SDL_GetRGBA(pixtab[j * width + i], format, &r, &g, &b, &a);
                if (r > max)
                    max = r;
            }
        }
    }

    return max;
}

void Thresholdhysteresis(SDL_Surface *image) {
    int seuil_sup = ComputeSeuil(image);
    int seuil_min = seuil_sup / 2;

    int height = image->h;
    int width = image->w;
    const SDL_PixelFormat *format = image->format;
    Uint32 *pixtab = image->pixels;

    Uint8 col;
    Uint8 trash;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            SDL_GetRGBA(pixtab[y * width + x], format, &col, &trash, &trash,
                        &trash);

            if (col > seuil_sup)
                col = 255;
            else if (col < seuil_min)
                col = 0;
            else
                col = 127;

            pixtab[y * width + x] =
                SDL_MapRGBA(format, (Uint8)col, (Uint8)col, (Uint8)col, 255);
        }
    }

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            SDL_GetRGBA(pixtab[y * width + x], format, &col, &trash, &trash,
                        &trash);

            if (col == 127) {
                Uint8 max = Max_val_Neighbor(image, x, y);
                if (max == 255)
                    col = 255;
                else
                    col = 0;
                pixtab[y * width + x] = SDL_MapRGBA(
                    format, (Uint8)col, (Uint8)col, (Uint8)col, 255);
            } else
                continue;
        }
    }
}

// Calcul la somme des éléments d'un tableau de iind a indf
int SumTab(int tab[], int indd, int indf) {
    int res = 0;
    for (int i = indd; i < indf; i++) {
        res += tab[i];
    }
    return res;
}

// calcul l'Histogram de l'image
static int *Histogram_Seuil(SDL_Surface *image) {
    int *histo = calloc(256, sizeof(int));

    int height = image->h;
    int width = image->w;
    const SDL_PixelFormat *format = image->format;
    Uint32 *pixtab = image->pixels;

    Uint8 r, g, b, a;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            SDL_GetRGBA(pixtab[y * width + x], format, &r, &g, &b, &a);
            histo[r] += 1;
        }
    }
    return histo;
}

// Récupère le pixel max et min d'une image
void Max_Min_in_pic(SDL_Surface *image, Uint8 *max, Uint8 *min) {
    int height = image->h;
    int width = image->w;
    const SDL_PixelFormat *format = image->format;
    Uint32 *pixtab = image->pixels;

    Uint8 r, g, b, a;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            SDL_GetRGBA(pixtab[y * width + x], format, &r, &g, &b, &a);
            if (r > *max)
                *max = r;
            if (r < *min)
                *min = r;
        }
    }
}

// ComputeSeuil utilise la méthode d'Otsu
Uint8 ComputeSeuil(SDL_Surface *image) {
    int height = image->h;
    int width = image->w;

    int threshold = 0, sum = 0, sum_back = 0, weight_back = 0;
    int total_pix = height * width;
    double max = 0.0;

    int *Histogram = Histogram_Seuil(image);
    for (int i = 0; i < 256; ++i) {
        sum += i * Histogram[i];
    }

    for (int i = 0; i < 256; ++i) {
        weight_back += Histogram[i];
        int weight_fore = total_pix - weight_back;
        if (weight_back == 0 || weight_fore == 0)
            continue;
        sum_back += i * Histogram[i];
        int sum_fore = sum - sum_back;

        double mean_back = sum_back / (double)weight_back;
        double mean_fore = sum_fore / (double)weight_fore;
        double mean_diff = mean_back - mean_fore;

        double var =
            mean_diff * mean_diff * (double)weight_back * (double)weight_fore;
        if (var > max) {
            max = var;
            threshold = i;
        }
    }

    free(Histogram);

    return (Uint8)threshold;
}
