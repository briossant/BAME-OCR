#include "ImageProcessing/ImageProcess.h"
#include "NeuralNetwork/network/NeuralNetwork.h"
#include "SudokuSolver/Sudoku_Solver.h"
#include <SDL2/SDL_image.h>

#define DEFAULT_NN "sudoku.nn"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: ./ocr <image path> <network path (optional)>\n");
        return 1;
    }
    char *nn_path = DEFAULT_NN;
    if (argc >= 3) {
        nn_path = argv[2];
    }

    Network network = LoadNetwork(nn_path);

    SDL_Surface *image = SDL_Start(argv[1]);

    SDL_PixelFormat *format = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888);
    SDL_Surface *image_copy = SDL_ConvertSurface(image, format, 0);

    image = Canny(image);
    int ho_mat_size = -1;
    int *ho_mat = hough_transform(image, &ho_mat_size);
    int *grid_corner = GridDetection(ho_mat, ho_mat_size);

    int sdk_grid[9][9]; // risky shit malloc may be better
    int grid_coo[9][9]; // risky shit malloc may be better

    int box_size_x = (grid_corner[2] - grid_corner[0]) / 9;
    int box_size_y = (grid_corner[3] - grid_corner[1]) / 9;

    for (int x = 0; x < 9; ++x) {
        for (int y = 0; y < 9; ++y) {
            int image_x = grid_corner[0] + x * box_size_x;
            int image_y = grid_corner[1] + y * box_size_y;
            // remove offseting from Resize_crop and do it after void_square
            SDL_Surface *box_img =
                Resize_crop(image_copy, image_x, image_y, image_x + box_size_x,
                            image_y + box_size_y);
            grid_coo[y][x] =
                image_y * image->w + image_x; // grid of coordinates of each box

            char *kokok;
            Balance(box_img);
            if (!void_square(box_img)) {
                asprintf(&kokok, "tmp/empty-%d-%d.png", x, y);
                IMG_SavePNG(box_img, kokok);
                sdk_grid[y][x] = 0;
                continue;
            }
            box_img = CenterNumber(box_img);
            asprintf(&kokok, "tmp/full-%d-%d.png", x, y);
            IMG_SavePNG(box_img, kokok);

            Matrix inputs = image_to_matrix(box_img);
            // MatPrint(inputs);
            int prediction = Predict(network, inputs);
            if (prediction == 0) {
                printf("predicted 0 which should not happen, replacing by 1\n");
                prediction = 1;
            }
            sdk_grid[y][x] = prediction;
        }
    }

    printgrid(sdk_grid);
    int copy_grid[9][9];

    for (int x = 0; x < 9; ++x)
        for (int y = 0; y < 9; ++y)
            copy_grid[y][x] = sdk_grid[y][x];

    if (SSudo(sdk_grid, 0, 0) != 1) {
        printf("No solution found\n");
        return 0;
    }

    for (int x = 0; x < 9; ++x)
        for (int y = 0; y < 9; ++y)
            if (copy_grid[y][x] == 0) {
                int start = grid_coo[y][x];
                int end = start + box_size_x + box_size_y * image_copy->w;
                get_green_number(image_copy, sdk_grid[y][x], start, end,
                                 "ImageProcessing/");
            }

    IMG_SavePNG(image_copy, "BAME-here's-your-sudoku!.png");
    printf("Successful bb\n");
    printgrid(sdk_grid);
    return 0;
}
