#include <SDL2/SDL.h>
/*
// Function to get the pixel value at (x, y) from an SDL_Surface
Uint32 SDL_GetPixel(SDL_Surface *surface, int x, int y) {
  int bpp = surface->format->BytesPerPixel;
  Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;
  return *(Uint32 *)p;
}

// Function to set the pixel value at (x, y) in an SDL_Surface
void SDL_SetPixel(SDL_Surface *surface, int x, int y, Uint32 pixel) {
  int bpp = surface->format->BytesPerPixel;
  Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;
  *(Uint32 *)p = pixel;
}

int main(int argc, char *argv[]) {

  // Load Figure 1 and Figure 2 images
  SDL_Surface *figure1 = SDL_LoadBMP("figure1.bmp"); // green number
  SDL_Surface *figure2 = SDL_LoadBMP("figure2.bmp"); // sudoku image

  if (figure1->w != figure2->w || figure1->h != figure2->h) {
    printf("both images must have the same dimensions");
    return 1;
  }

  // Create a new surface for the result
  SDL_Surface *result =
      SDL_CreateRGBSurface(0, figure1->w, figure1->h, 32, 0, 0, 0, 0);

  // Overlay images
<<<<<<< HEAD
  for (int x = 0; x < figure1->w; x++) {
    for (int y = 0; y < figure1->h; y++) {
      Uint32 pixel1 = SDL_GetPixel(figure1, x, y);
      Uint32 pixel2 = SDL_GetPixel(figure2, x, y);
      Uint8 red = (pixel1 >> 16) & 0xFF;  // Extract the red channel
      Uint8 green = (pixel1 >> 8) & 0xFF; // Extract the green channel
      Uint8 blue = pixel1 & 0xFF;         // Extract the blue channel

      if (red == 0 && green == 255 && blue == 0) {

        SDL_SetPixel(result, x, y, pixel1);
      } else {
        SDL_SetPixel(result, x, y,
                     pixel2); // try pixel2->0 supposed to be transparent
      }
    }
  }
  // Display or save the resulti ???

  SDL_FreeSurface(figure1);
  SDL_FreeSurface(figure2);
  SDL_FreeSurface(result);
  SDL_Quit();

  return 0;
=======

  // Display or save the resulti ???

  SDL_FreeSurface(figure1);
  SDL_FreeSurface(figure2);
  SDL_FreeSurface(result);
  SDL_Quit();

  return 0;
}
*/

void overlaying_image(SDL_Surface *image, SDL_Surface *image2) {

  int height = image->h;
  int width = image->w;
  const SDL_PixelFormat *format = image->format;
  Uint32 *pixtab = image->pixels;
  Uint32 *pixtab2 = image2->pixels;
  Uint8 r, g, b, a;
  Uint8 r1, g1, b1, a1;

  for (int x = 0; x < width; x++) {
    for (int y = 0; y < height; y++) {
      SDL_GetRGBA(pixtab[y * width + x], format, &r, &g, &b, &a);
      SDL_GetRGBA(pixtab2[y * width + x], format, &r1, &g1, &b1, &a1);
      if (r1 == 0 && g1 == 255 && b1 == 0) {

        pixtab[y * width + x] = SDL_MapRGBA(format, 0, 255, 0, 255);
      } else {
        //        SDL_SetPixel(result, x, y,
        //                     pixel2); // try pixel2->0 supposed to be
        //                     transparent
        // pixtab[y * width + x] = SDL_MapRGBA(format, 0, 0, 0, 0);
      }
    }
  }
>>>>>>> origin/NN-MATRICES
}
