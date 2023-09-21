//------------------------------
// Include all files, parse
// command line arguments, and
// execute right functions.
//------------------------------

//---Includes
#include "ImageProcessing/ImageProcess.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <err.h>

//---Init
char version[] = "v1.0";

void print_usage(char* argv0) { // TODO: improve
    errx(1, "Usage: %s <file to open> <file to save>\n", argv0);
    // printf("Usage : %s [-h] [-v] [-d] [-a ALGO] [-H HEUR] FILE\n", argv0);
}

void print_help() // TODO: improve
{
    printf("Usage: ocr [options] file...");
}

void loader()
{
    // TODO
}

SDL_Surface* SDL_Start(char* filename, int option)
{
    // SDL Init

    if(0 != SDL_Init(SDL_INIT_VIDEO))
    {
        errx(1, "Unable to initialize SDL: %s\n", SDL_GetError());
    }

    SDL_Surface* image = NULL;
    image = IMG_Load(filename);
    if (image == NULL) 
    {
        printf("Can't load image : %s\n", IMG_GetError());
        return;
    }

    SDL_PixelFormat* format = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888);
    SDL_Surface *image_converted = SDL_ConvertSurface(image, format, 0);
    SDL_FreeSurface(image);
    return image_converted;
}

    // GreyScale(image_converted);
    // BlacknWhite(image_converted);
    // Contrast(image_converted);
    // GaussianBlur(image_converted);
    // Bright(image_converted);
    // ArroundGaussianBlur(image_converted);
    // image_converted = Rotate(image_converted, 35);
    // Canny(image_converted);

    // Output

int SDL_Output(SDL_Surface *image_converted)
    if (IMG_SavePNG(image_converted, argv[2]) != 0) 
    {
        printf("Error when trying to save the image : %s\n", IMG_GetError());
    }

    SDL_FreeSurface(image_converted);
    
    IMG_Quit();
    SDL_Quit();

    return 1;
}

int parse(int argc, char** argv)
{
    //---Parsing
    if (argc == 1) //No arguments
    {
        print_logo();
        print_usage(argv[0]);
        printf("OCR: error: the following argument is requied: PATH\n");
        return 1;
    }

    for (int k = 1 ; k < argc ; k++) 
    {
        if (strcmp(argv[k], "-h") == 0 || strcmp(argv[k], "--help") == 0) //Help
        { 
            print_help(argv[0]);
            return 0;
        }
        else if (strcmp(argv[k], "-v") == 0 || strcmp(argv[k], "--version") == 0) //Version
        {
            printf("OCR version : %s\n", version);
            return 0;
        }
        // else if (strcmp(argv[k], "-l") == 0 || strcmp(argv[k], "--loader") == 0) //Loader
        // {
        //     loader(argv[2]);
        //     return 0;c 
        // }

        else
        {
            printf("sdl: error: unrecognized command-line option '%s'\n", argv[k]);
            return 0;
        }
    }

    return 0;
}

int main(int argc, char** argv) {
    //---Parser
    return parse(argc, argv);
}