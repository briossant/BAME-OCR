#include "ImageProcess.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_surface.h>
#include <err.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Init
char version[] = "2.0";

void print_error(char *argv0, char *argv_error) {
  printf("%s: error: unrecognized command-line option '%s'\n", argv0,
         argv_error);
}

void print_fatal_error(char *argv0) {
  printf("%s: fatal error: no input or output files\n", argv0);
}

void print_help(char *argv0) {
  printf("Usage: %s [options]\n", argv0);
  printf("Options:\n");
  // printf("a, all                   Apply all the filters for the OCR\n");
  // //
  // TODO: Complete or remove
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
  printf("gd, griddetection [a][b] Create markers for the grid\n");
  printf("ar, autorotate           Rotate the image\n");
  printf("cr, crop [a][b][c][d]    Crop between the 2 points (a, b) (c, d)\n");
  printf("gn, greennb [a][b][c][d] Draw 1 between the 2 points (a, b) (c, "
         "d)\n"); // TODO: all the digits
}

typedef struct {
  char *input;
  int *argv;
  int angle;
  int argc;
  char *output;
  int stop;
  int threshold;
  int state;
  pair p1;
  pair p2;
  pair p3;
  pair p4;
} uplet;

uplet sort_argv(char *argv0, char *input, int argc) {
  uplet res;
  res.stop = 0;
  res.argv = (int *)malloc((argc) * sizeof(int)); // FIXME
  res.argc = argc - 2;
  for (int i = 0; i < res.argc; i++)
    res.argv[i] = -1;

  int no_image_input = 1;
  int no_image_output = 1;

  char *arg = strtok(input, " ");
  ;
  int i = 0;

  do {
    if (strcmp(arg, "i") == 0 || strcmp(arg, "input") == 0) // Input file
    {
      if (!no_image_input) {
        printf("%s: fatal error: to many input files\n", argv0);
        res.stop = -1;
        break;
      }
      arg = strtok(NULL, " ");
      if (arg == NULL) {
        res.stop = -1;
        break;
      } else
        res.input = arg;
      no_image_input = 0;
    } else if (strcmp(arg, "o") == 0 ||
               strcmp(arg, "output") == 0) // Output file
    {
      if (!no_image_output) {
        printf("%s: fatal error: too many output files\n", argv0);
        res.stop = -1;
        break;
      }
      arg = strtok(NULL, " ");
      if (arg == NULL) {
        res.stop = -1;
        break;
      } else
        res.output = arg;
      no_image_output = 0;
    } else if (strcmp(arg, "h") == 0 || strcmp(arg, "help") == 0) // Help
    {
      print_help(argv0);
      res.stop = 1;
      break;
    } else if (strcmp(arg, "v") == 0 || strcmp(arg, "version") == 0) // Version
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
    else if (strcmp(arg, "g") == 0 ||
             strcmp(arg, "greyscale") == 0) // GreyScale
    {
      res.argv[i] = 2;
    } else if (strcmp(arg, "bw") == 0 ||
               strcmp(arg, "blackwhite") == 0) // BlacknWhite
    {
      res.argv[i] = 3;
    } else if (strcmp(arg, "c") == 0 ||
               strcmp(arg, "contrast") == 0) // Contrast
    {
      res.argv[i] = 1;
    } else if (strcmp(arg, "gb") == 0 ||
               strcmp(arg, "gaussianblur") == 0) // GaussianBlur
    {
      res.argv[i] = 4;
    } else if (strcmp(arg, "b") == 0 || strcmp(arg, "bright") == 0) // Bright
    {
      res.argv[i] = 0;
    } else if (strcmp(arg, "ag") == 0 ||
               strcmp(arg, "arroundgaussianblur") == 0) // ArroundGaussianBlur
    {
      res.argv[i] = 5;
    } else if (strcmp(arg, "r") == 0 || strcmp(arg, "rotate") == 0) // Rotate
    {
      res.argv[i] = 7;
      arg = strtok(NULL, " ");
      if (arg == NULL) {
        res.stop = -2;
        break;
      }
      res.angle = atoi(arg);
    } else if (strcmp(arg, "ca") == 0 || strcmp(arg, "canny") == 0) // Canny
    {
      res.argv[i] = 6;
    } else if (strcmp(arg, "gd") == 0 ||
               strcmp(arg, "griddetection") == 0) // Grid Detection
    {
      res.argv[i] = 9;
    } else if (strcmp(arg, "ho") == 0 || strcmp(arg, "hough") == 0) // Hough
    {
      res.argv[i] = 10;
    } else if (strcmp(arg, "ar") == 0 ||
               strcmp(arg, "autorotate") == 0) // Auto_Rotate
    {
      res.argv[i] = 8;
    } else if (strcmp(arg, "cr") == 0 || strcmp(arg, "crop") == 0) // Crop
    {
      res.argv[i] = 11;
      arg = strtok(NULL, " ");
      if (arg == NULL) {
        res.stop = -2;
        break;
      }
      res.p1.x = atoi(arg); // TODO: Opti
      arg = strtok(NULL, " ");
      if (arg == NULL) {
        res.stop = -2;
        break;
      }
      res.p1.y = atoi(arg);
      arg = strtok(NULL, " ");
      if (arg == NULL) {
        res.stop = -2;
        break;
      }
      res.p2.x = atoi(arg);
      arg = strtok(NULL, " ");
      if (arg == NULL) {
        res.stop = -2;
        break;
      }
      res.p2.y = atoi(arg);
    } else if (strcmp(arg, "gn") == 0 ||
               strcmp(arg, "greennb") == 0) // Green Number Overlay
    {
      res.argv[i] = 12;
      arg = strtok(NULL, " ");
      if (arg == NULL) {
        res.stop = -2;
        break;
      }
      res.p3.x = atoi(arg);
      arg = strtok(NULL, " ");
      if (arg == NULL) {
        res.stop = -2;
        break;
      }
      res.p3.y = atoi(arg);
      arg = strtok(NULL, " ");
      if (arg == NULL) {
        res.stop = -2;
        break;
      }
      res.p4.x = atoi(arg);
      arg = strtok(NULL, " ");
      if (arg == NULL) {
        res.stop = -2;
        break;
      }
      res.p4.y = atoi(arg);
    } else {
      print_error(argv0, arg);
      res.stop = 1;
      break;
    }
    i++;
  } while ((arg = strtok(NULL, " ")) != NULL);

  if ((no_image_input || no_image_output) && res.stop != 1)
    res.stop = -1;

  return res;
}

int ImageProcess(uplet argv) {
  // Do the action in order
  SDL_Surface *image_converted = SDL_Start(argv.input);
  if (image_converted == NULL)
    return 1;

  int ho_matrix_size = -1;
  int *ho_matrix = NULL;
  for (int i = 0; i < argv.argc; i++) {
    // printf("%d\n", argv.argv[i]);
    if (argv.argv[i] == 2) // GreyScale
    {
      GreyScale(image_converted);
    } else if (argv.argv[i] == 3) // BlacknWhite
    {
      BlacknWhite(image_converted);
      // InvertColor(image_converted);
      //  Balance(image_converted);
    } else if (argv.argv[i] == 1) // Contrast
    {
      Contrast(image_converted);
    } else if (argv.argv[i] == 4) // GaussianBlur
    {
      GaussianBlur(image_converted);
    } else if (argv.argv[i] == 0) // Bright
    {
      Bright(image_converted);
    } else if (argv.argv[i] == 5) // Arround Gaussian Blur
    {
      ArroundGaussianBlur(image_converted);
    } else if (argv.argv[i] == 6) // Canny
    {
      image_converted =
          Canny(image_converted, image_converted->w, image_converted->h);
    } else if (argv.argv[i] == 7) // Rotate
    {
      image_converted = Rotate(image_converted, argv.angle);
    } else if (argv.argv[i] == 8) // Auto Rotate
    {
      // Auto_Rotate(image_converted);
    } else if (argv.argv[i] == 9) // GridDetection
    {
      if (ho_matrix_size == -1)
        errx(1, "grid detection need hough to be run before");
      int *grid = GridDetection(ho_matrix, ho_matrix_size);

      Uint32 color = SDL_MapRGBA(image_converted->format, 0, 255, 0, 255);
      draw_line(image_converted, grid[0], grid[1], grid[2], grid[3], color);
    } else if (argv.argv[i] == 10) // Hough
    {
      double *tmp = hough_transform(image_converted, &ho_matrix_size);
      ho_matrix = TransformHoughPolarToPoints(tmp, ho_matrix_size);
    } else if (argv.argv[i] == 11) // Crop
    {
      // image_converted = Crop(image_converted, argv.p1, argv.p2);
      image_converted = Resize_crop(image_converted, argv.p1.x, argv.p1.y,
                                    argv.p2.x, argv.p2.y);

      // Balance(image_converted);
    } else if (argv.argv[i] == 12) // Contrast
    {
      get_green_number(image_converted, 1,
                       argv.p3.x + image_converted->w * argv.p3.y,
                       argv.p4.x + image_converted->w * argv.p4.y, "./");
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

int main(int argc, char **argv) {
  (void)argc; // Unused variable

  print_logo();

  if (0 != SDL_Init(SDL_INIT_VIDEO)) {
    errx(1, "Unable to initialize SDL: %s\n", SDL_GetError());
  }

  // Parser
  int res = 0;

  while (4 * 10 + (6 - 2) + 8 / 2 - 3 + (5 % 2) + (5 << 1) - (7 / 3) + 1 -
         13) // While 42
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
    if (tmp.stop == 0)
      res = ImageProcess(tmp);
    else if (tmp.stop == -1)
      print_fatal_error(argv[0]);
    else if (tmp.stop == -2)
      printf("%s: fatal error: no good arguments\n", argv[0]);
    free(input);
  }

  return res;
}
