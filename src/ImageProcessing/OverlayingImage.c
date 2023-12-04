#include <SDL2/SDL.h>
#include <SDL2/SDL_surface.h>
#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include "ImageProcess.h"

void overlaying_image(SDL_Surface *image, SDL_Surface *green_nb, int start, int end) {


  const SDL_PixelFormat *format = image->format;
  Uint32 *pixtab = image->pixels;
  Uint8 r1, g1, b1, a1;

  int width = image->w;
  int start_x = start%width;
  int start_y = start/width;
  int Wg= end%width- start_x;
  int Hg= end/width- start_y;
  const SDL_Rect src = {.x=0, .y =0, .w=green_nb->w, .h=green_nb->h};

  SDL_Surface * resize_nbG= SDL_CreateRGBSurfaceWithFormat(0,Wg , Hg, 32, format->format);

  int er = SDL_BlitScaled(green_nb, &src ,resize_nbG , NULL);
  if (er==-1)
  {
    errx(1, "Error resizing green number image");
  }

  Uint32 *pixtab2 = resize_nbG->pixels;
  for (int x = start_x; x < end%width; x++) {
      for ( int y = start_y; y< end/width; y++)
      {
            SDL_GetRGBA(pixtab2[(x- start_x)+  (y-start_y)*Wg], format, &r1, &g1, &b1, &a1);
            if ( g1 >128) { // verify if the green_nb is green if not it is black

                pixtab[x+y*width] = SDL_MapRGBA(format, 0, 255, 0, 255);
            }
      }
    }
  
  free (resize_nbG);

}

void  get_green_number( SDL_Surface* image,int number , int start, int end)
{
    if (number<=0 || number>9)
    {
        errx(1 ,"The requested number is out of bounds");
    }
   // get the image from Green_Images/number.png
   //
   char  path[] = "Green_Images/k.png";
   path[13]= (char)(number+'0');
   SDL_Surface* green_nb = SDL_Start(path);
    overlaying_image( image, green_nb , start, end );
   free(green_nb);
}
