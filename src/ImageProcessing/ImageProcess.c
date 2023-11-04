#include "ImageProcess.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_surface.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <err.h>
#include <math.h>

// Init
char version[] = "2.0";

void print_logo()
{
    printf(" ▄▄▄▄    ▄▄▄       ███▄ ▄███▓▓█████     ▒█████   ▄████▄   ██▀███  \n");
    printf("▓█████▄ ▒████▄    ▓██▒▀█▀ ██▒▓█   ▀    ▒██▒  ██▒▒██▀ ▀█  ▓██ ▒ ██▒\n");
    printf("▒██▒ ▄██▒██  ▀█▄  ▓██    ▓██░▒███      ▒██░  ██▒▒▓█    ▄ ▓██ ░▄█ ▒\n");
    printf("▒██░█▀  ░██▄▄▄▄██ ▒██    ▒██ ▒▓█  ▄    ▒██   ██░▒▓▓▄ ▄██▒▒██▀▀█▄  \n");
    printf("░▓█  ▀█▓ ▓█   ▓██▒▒██▒   ░██▒░▒████▒   ░ ████▓▒░▒ ▓███▀ ░░██▓ ▒██▒\n");
    printf("░▒▓███▀▒ ▒▒   ▓▒█░░ ▒░   ░  ░░░ ▒░ ░   ░ ▒░▒░▒░ ░ ░▒ ▒  ░░ ▒▓ ░▒▓░\n");
    printf("▒░▒   ░   ▒   ▒▒ ░░  ░      ░ ░ ░  ░     ░ ▒ ▒░   ░  ▒     ░▒ ░ ▒░\n");
    printf(" ░    ░   ░   ▒   ░      ░      ░      ░ ░ ░ ▒  ░          ░░   ░ \n");
    printf(" ░            ░  ░       ░      ░  ░       ░ ░  ░ ░         ░     \n");
    printf("      ░                                         ░                 \n\n\n");
}

void print_error(char* argv0, char* argv_error)
{
    printf("%s: error: unrecognized command-line option '%s'\n", argv0, argv_error);
}

void print_fatal_error(char* argv0)
{
    printf("%s: fatal error: no input or output files\n", argv0);
}

void print_help(char* argv0)
{
    printf("Usage: %s [options]\n", argv0);
    printf("Options:\n");
    printf("a, all                   Apply all the filters for the OCR\n");
    printf("i, input [filename]      Input file\n");
    printf("o, output [filename]     Output file\n");
    printf("h, help                  Display this help message\n");
    printf("v, version               Display version information\n");
    printf("g, greyscale             Convert the image to grayscale\n");
    printf("bw, blackwhite           Convert the image to black and white\n");
    printf("c, contrast              Adjust the image contrast\n");
    printf("gb, gaussianblur         Apply Gaussian blur to the image\n");
    printf("b, bright                Adjust the image brightness\n");
    printf("ag, arroundgaussianblur  Apply Gaussian blur to the image\n");
    printf("r, rotate [angle]        Rotate the image with the angle\n");
    printf("ca, canny                Apply Canny filters to the image\n");
    printf("gd, griddetection        Create markers for the grid\n");
}

// Input
SDL_Surface* SDL_Start(char* filename)
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
        printf("%s: cannot find %s: No such file or directory\n", filename, filename);
        // printf("Can't load image : %s\n", IMG_GetError());
        return NULL;
    }

    SDL_PixelFormat* format = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888);
    SDL_Surface *image_converted = SDL_ConvertSurface(image, format, 0);
    SDL_FreeSurface(image);
    return image_converted;
}

// Output
int SDL_Output(SDL_Surface *image_converted, char* file)
{
    if (IMG_SavePNG(image_converted, file) != 0) 
    {
        printf("Error when trying to save the image : %s\n", IMG_GetError());
    }

    SDL_FreeSurface(image_converted);
    
    IMG_Quit();
    SDL_Quit();

    return 1;
}

typedef struct {
    char* input;
    int* argv;
    int angle;
    int argc;
    char* output;
    int stop;
} uplet;

uplet sort_argv(char* argv0, char* input, int argc)
{
    uplet res;
    res.stop = 0;
    res.argv = (int*)malloc((argc)*sizeof(int));
    res.argc = argc-2;
    for (int i = 0; i < res.argc; i++)
        res.argv[i] = -1;
    
    int no_image_input = 1;
    int no_image_output = 1;

    char* arg = strtok(input, " ");;
    int i = 0;

    do
    {
        if (strcmp(arg, "i") == 0 || strcmp(arg, "input") == 0) //Input file
        {
            if(!no_image_input)
            {
                printf("%s: fatal error: to many input files\n", argv0);
                res.stop = -1;
                break;
            }
            arg = strtok(NULL, " ");
            if (arg == NULL)
            {
                res.stop = -1;
                break;
            }
            else
                res.input = arg;
            no_image_input = 0;
        }
        else if (strcmp(arg, "o") == 0 || strcmp(arg, "output") == 0) //Output file
        {
            if(!no_image_output)
            {
                printf("%s: fatal error: too many output files\n", argv0);
                res.stop = -1;
                break;
            }
            arg = strtok(NULL, " ");
            if (arg == NULL)
            {
                res.stop = -1;
                break;
            }
            else
                res.output = arg;
            no_image_output = 0;
        }
        else if (strcmp(arg, "h") == 0 || strcmp(arg, "help") == 0) //Help
        {
            print_help(argv0);
            res.stop = 1;
            break;
        }
        else if (strcmp(arg, "v") == 0 || strcmp(arg, "version") == 0) //Version
        {
            printf("%s %s\n", argv0, version);
            res.stop = 1;
            break;
        }
        // else if (strcmp(arg, "a") == 0 || strcmp(arg, "all") == 0)
        // {
        //     res.argv[i++] = 2;
        //     res.argv[i++] = 3;
        //     res.argv[i++] = 4;
        //     res.argv[i++] = 5;
        //     res.argv[i++] = 6;
        //     res.argv[i] = 7;
        // }
        else if (strcmp(arg, "g") == 0 || strcmp(arg, "greyscale") == 0) //GreyScale
        {
            res.argv[i] = 2;
        }
        else if (strcmp(arg, "bw") == 0 || strcmp(arg, "blackwhite") == 0) //BlacknWhite
        {
            res.argv[i] = 3;
        }
        else if (strcmp(arg, "c") == 0 || strcmp(arg, "contrast") == 0) //Contrast
        {
            res.argv[i] = 1;
        }
        else if (strcmp(arg, "gb") == 0 || strcmp(arg, "gaussianblur") == 0) //GaussianBlur
        {
            res.argv[i] = 4;
        }
        else if (strcmp(arg, "b") == 0 || strcmp(arg, "bright") == 0) //Bright
        {
            res.argv[i] = 0;
        }
        else if (strcmp(arg, "ag") == 0 || strcmp(arg, "arroundgaussianblur") == 0) //ArroundGaussianBlur
        {
            res.argv[i] = 5;
        }
        else if (strcmp(arg, "r") == 0 || strcmp(arg, "rotate") == 0) //Rotate
        {
            res.argv[i++] = 7;
            arg = strtok(NULL, " ");
            if (arg == NULL)
            {
                res.stop = -1;
                break;
            }
            res.angle = atoi(arg);
        }
        else if (strcmp(arg, "ca") == 0 || strcmp(arg, "canny") == 0) //Canny
        {
            res.argv[i] = 6;
        }
        else if (strcmp(arg, "dg") == 0 || strcmp(arg, "griddetection") == 0) //Canny
        {
            res.argv[i] = 8;
        }
        else
        {
            print_error(argv0, arg);
            res.stop = 1;
            break;
        }
    } while ((arg = strtok(NULL, " ")) != NULL);

    if ((no_image_input || no_image_output) && res.stop != 1)
        res.stop = -1;

    return res;
}

int ImageProcess(uplet argv)
{
    // Do the action in order

    SDL_Surface* image_converted = SDL_Start(argv.input);
    if(image_converted == NULL)
        return 1;

    for(int i = 0; i < argv.argc; i++)
    {
        printf("%d\n", argv.argv[i]);
        if (argv.argv[i] == 2) //GreyScale
        {
            GreyScale(image_converted);
        }
        else if (argv.argv[i] == 3) //BlacknWhite
        {
            BlacknWhite(image_converted);
        }
        else if (argv.argv[i] == 1) //Contrast
        {
            Contrast(image_converted);
        }
        else if (argv.argv[i] == 4) //GaussianBlur
        {
            GaussianBlur(image_converted);
        }
        else if (argv.argv[i] == 0) //Bright
        {
            Bright(image_converted);
        }
        else if (argv.argv[i] == 5) //ArroundGaussianBlur
        {
            ArroundGaussianBlur(image_converted);
        }
        else if (argv.argv[i] == 6) //Canny
        {
            image_converted = Canny(image_converted);
        }
        else if (argv.argv[i] == 7) //Rotate
        { 
            image_converted = Rotate(image_converted, argv.angle);
        }
        else if (argv.argv[i] == 7) //GridDetection
        { 
            image_converted = hough_transform(image_converted, 300);
        }
    }

    SDL_Output(image_converted, argv.output);

    return 0;
}

size_t GetArgc(char *input) {
    char *p = input;
    size_t res = 0;
    while (*p != 0)
        if (*(p++) == ' ')
            ++res;
    return res;
}

int main(int argc, char** argv) {
    (void)argc; // Unused variable

    // Parser
    int res = 0;

    while(4 * 10 + (6 - 2) + 8 / 2 - 3 + (5 % 2) + (5 << 1) - (7 / 3) + 1 - 13)
    {
        char *input = NULL;
        size_t len = 0;
        printf("〉");
        int input_argc = getline(&input, &len, stdin);
        if (input_argc == 1)
            continue;

        // remove trailing newline from user inputs
        input[strcspn(input, "\n")] = 0;

        // get number of argv and command name
        // size_t argc = GetArgc(input);

        uplet tmp = sort_argv(argv[0], input, input_argc);
        if(tmp.stop == 0)
            res = ImageProcess(tmp);
        else if (tmp.stop == -1)
            print_fatal_error(argv[0]);
        free(input);
    }

    return res;
}