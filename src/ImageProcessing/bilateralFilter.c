#include "ImageProcess.h"

float distance(int x, int y, int i, int j) {
    return (float)(sqrt(pow(x - i, 2) + pow(y - j, 2)));
}

double gaussian(float x, double sigma) {
    return exp(-(pow(x, 2)) / (2 * pow(sigma, 2))) / (2 * M_PI * pow(sigma, 2));
}

void applyBilateralFilter(SDL_Surface *image, SDL_Surface *filteredImage, int x,
                          int y, int diameter, double sigmaI, double sigmaS) {
    double iFiltered = 0;
    double wP = 0;
    int neighbor_x = 0;
    int neighbor_y = 0;
    int width = image->w;
    int height = image->h;
    int half = diameter / 2;
    const SDL_PixelFormat *format = image->format;
    Uint32 *pixtab = image->pixels;
    Uint32 *fpixtab = filteredImage->pixels;
    Uint8 r, g, b, a;
    Uint8 r2, g2, b2, a2;

    SDL_GetRGBA(pixtab[y * width + x], format, &r2, &g2, &b2, &a2);
    for (int i = 0; i < diameter; i++) {
        for (int j = 0; j < diameter; j++) {

            neighbor_x = x - (half - i);
            neighbor_y = y - (half - j);
            if (neighbor_x < 0 || neighbor_x >= width || neighbor_y < 0 ||
                neighbor_y >= height)
                continue;
            SDL_GetRGBA(pixtab[neighbor_y * width + neighbor_x], format, &r, &g,
                        &b, &a);
            double gi = gaussian(r - r2, sigmaI);
            double gs =
                gaussian(distance(x, y, neighbor_x, neighbor_y), sigmaS);
            double w = gi * gs;
            iFiltered = iFiltered + r * w;
            wP = wP + w;
        }
    }
    iFiltered = iFiltered / wP;
    fpixtab[y * width + x] =
        SDL_MapRGBA(format, iFiltered, iFiltered, iFiltered, 255);
}

SDL_Surface *bilateralFilterOwn(SDL_Surface *image, int diameter, double sigmaI,
                                double sigmaS) {
    const SDL_PixelFormat *format = image->format;
    SDL_Surface *filteredImage = SDL_ConvertSurface(image, format, 0);
    int width = image->w;
    int height = image->h;

    for (int i = 2; i < height - 2; i++) {
        for (int j = 2; j < width - 2; j++) {
            applyBilateralFilter(image, filteredImage, j, i, diameter, sigmaI,
                                 sigmaS);
        }
    }
    return filteredImage;
}
