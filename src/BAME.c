#include "ImageProcessing/ImageProcess.h"
#include "NeuralNetwork/network/NeuralNetwork.h"
#include "SudokuSolver/Sudoku_Solver.h"
#include "Interface/interface.h"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_surface.h>
#include <stdio.h>
#include <stdlib.h>

void *BAME(void *data) {

    print_logo();
    ThreadParameters *parameters = data;
    printf("Solving your sudoku in %s\n", parameters->filename);

    Network network = LoadNetwork(DEFAULT_NN);

    SDL_Surface *image = SDL_Start(parameters->filename);
    if (image == NULL)
        err(0, "Image not found");

    int old_width = image->w;
    int old_height = image->h;

    // step 1
    image = StandardizeImage(image);
    image = bilateralFilterOwn(image, 8, 100, 150);
    IMG_SavePNG(image, parameters->filename_resolved);

    SDL_PixelFormat *format = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888);
    SDL_Surface *image_copy = SDL_ConvertSurface(image, format, 0);

    if (parameters->step_index < 7)
        parameters->show_img();

    // step 2
    image = Canny(image, old_width, old_height);
    SDL_Surface *canny_copy = SDL_ConvertSurface(image, format, 0);

    IMG_SavePNG(image, parameters->filename_resolved);
    if (parameters->step_index < 7)
        parameters->show_img();

    // step 3
    int ho_mat_size = -1;
    double *ho_mat = hough_transform(image, &ho_mat_size);
    IMG_SavePNG(image, parameters->filename_resolved);
    SDL_FreeSurface(image);

    if (parameters->step_index < 7)
        parameters->show_img();

    // step 4
    double angle_to_rotate = parameters->angle;
    if (parameters->auto_rotate) {
        angle_to_rotate =
            GetImageAngleAndRotateHoughMatrix(ho_mat, ho_mat_size);
    }
    printf("Rotating the image %lf degrees\n", angle_to_rotate);
    image_copy = Rotate(image_copy, angle_to_rotate);
    SDL_Surface *image_copy_test = SDL_ConvertSurface(image_copy, format, 0);
    canny_copy = Rotate(canny_copy, angle_to_rotate);

    Uint32 *pixtab = image_copy_test->pixels;
    int height = image_copy_test->h;
    int width = image_copy_test->w;
    Uint8 r, g, b, a;
    Uint8 biais = ComputeSeuil(image_copy_test);
    if (biais < 10) {
        biais = 255;
    }
    for (int x = 0; x < width * height; x++) {
        SDL_GetRGBA(pixtab[x], format, &r, &g, &b, &a);

        if ((r + g + b) / 3 > biais) {
            r = 255;
            g = 255;
            b = 255;
        } else {
            r = 0;
            g = 0;
            b = 0;
        }

        // Pixel Update
        pixtab[x] = SDL_MapRGBA(format, r, g, b, a);
    }

    IMG_SavePNG(canny_copy, parameters->filename_resolved);
    if (parameters->step_index < 7)
        parameters->show_img();

    // step 5
    ho_mat = hough_transform(canny_copy, &ho_mat_size);
    int *ho_points = TransformHoughPolarToPoints(ho_mat, ho_mat_size);
    int *grid_corner = GridDetection(ho_points, ho_mat_size);

    if (grid_corner[0] < 0 || grid_corner[1] < 0) {
        printf(
            "Unable to find the sudoku grid... maybe your image is trash ??\n");
        if (parameters->step_index < 8)
            parameters->raise_error(
                "Unable to find a grid... maybe your image is trash ??\n");
    }

    Uint32 color = SDL_MapRGBA(image_copy->format, 255, 0, 255, 255);
    draw_line(canny_copy, grid_corner[0], grid_corner[1], grid_corner[2],
              grid_corner[3], color);
    IMG_SavePNG(canny_copy, parameters->filename_resolved);
    if (parameters->step_index < 7)
        parameters->show_img();

    // step 6
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
                Resize_crop(image_copy_test, image_x, image_y,
                            image_x + box_size_x, image_y + box_size_y);
            grid_coo[y][x] = image_y * image_copy->w +
                             image_x; // grid of coordinates of each box

            // char *kokok;
            Balance(box_img);
            if (!void_square(box_img)) {
                /*asprintf(&kokok, "tmp/empty-%d-%d.png", x, y);
                IMG_SavePNG(box_img, kokok);*/
                sdk_grid[y][x] = 0;
                continue;
            }
            /*asprintf(&kokok, "tmp/full-before-%d-%d.png", x, y);
            IMG_SavePNG(box_img, kokok);
            asprintf(&kokok, "tmp/full-%d-%d.png", x, y);
            IMG_SavePNG(box_img, kokok);*/

            box_img = CenterNumber(box_img);
            Matrix inputs = image_to_matrix(box_img);
            // MatPrint(inputs);
            int prediction = Predict(network, inputs);
            /*if (prediction == 0) {
                printf("predicted 0 which should not happen, replacing by 0\n");
                prediction = 0;
            }*/
            sdk_grid[y][x] = prediction;
        }
    }

    // step 7

    printf("Detected grid:\n");
    printgrid(sdk_grid);
    int copy_grid[9][9];

    for (int x = 0; x < 9; ++x)
        for (int y = 0; y < 9; ++y)
            copy_grid[y][x] = sdk_grid[y][x];

    if (SSudo(sdk_grid, 0, 0) != 1) {
        printf("No solution found, please solve it yourself  "
               "(just fill empty square with the right number)\n");
        if (parameters->step_index < 8) {
            int changed = parameters->validate_numbers(
                "No solution found please modifie the grid numbers in : "
                "src/SudokuSolver/grid_01\n Do not close this window unless "
                "you have already modified the grid or wish to ignore this "
                "message\n");
        } else
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

    IMG_SavePNG(image_copy, parameters->filename_resolved);
    if (parameters->step_index < 8)
        parameters->show_img();
    printf("\nSuccessful bb, BAME-OCR is sooo awsome!\n\n");
    printf("Solved grid:\n");
    printgrid(sdk_grid);
    printf("\nSolved image was saved as BAME.png\n");
    printf("\nYou're welcome,\nGoodbye!\n");
    return 0;
}
