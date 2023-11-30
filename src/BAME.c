#include "ImageProcessing/ImageProcess.h"
#include "NeuralNetwork/network/NeuralNetwork.h"
#include "SudokuSolver/Sudoku_Solver.h"

#define NN_FILENAME "sudoku.nn"

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("Usage: ./ocr <image path>\n");
    return 1;
  }

  Network network = LoadNetwork(NN_FILENAME);

  SDL_Surface *image = SDL_Start(argv[1]);

  SDL_PixelFormat *format = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888);
  SDL_Surface *image_copy = SDL_ConvertSurface(image, format, 0);

  image = Canny(image);
  int ho_mat_size = -1;
  int *ho_mat = hough_transform(image, &ho_mat_size);
  int *grid_corner = GridDetection(ho_mat, ho_mat_size);

  int sdk_grid[9][9]; // risky shit malloc may be better

  int box_size_x = (grid_corner[2] - grid_corner[0]) / 9;
  int box_size_y = (grid_corner[3] - grid_corner[1]) / 9;
  for (int x = 0; x < 9; ++x) {
    for (int y = 0; y < 9; ++y) {
      int image_x = grid_corner[0] + x * box_size_x;
      int image_y = grid_corner[1] + y * box_size_y;
      SDL_Surface *box_img =
          Resize_crop(image_copy, image_x, image_y, image_x + box_size_x,
                      image_y + box_size_y);

      Balance(box_img);
      if (!void_square(box_img)) {
        printf("void -> x: %d y: %d\n", x, y);
        sdk_grid[y][x] = 0;
        continue;
      }
      char *kokok;
      asprintf(&kokok, "temp%d-%d.png", x, y);
      IMG_SavePNG(box_img, kokok);

      Matrix inputs = image_to_matrix(box_img);
      // MatPrint(inputs);
      int prediction = Predict(network, inputs);
      sdk_grid[y][x] = prediction;
    }
  }

  printgrid(sdk_grid);
  if (SSudo(sdk_grid, 0, 0) != 1) {
    printf("No solution found\n");
    return 0;
  }
  printf("Successful bb\n");
  printgrid(sdk_grid);
  return 0;
}