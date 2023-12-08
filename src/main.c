#include "ImageProcessing/ImageProcess.h"
#include "NeuralNetwork/network/NeuralNetwork.h"
#include "SudokuSolver/Sudoku_Solver.h"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_surface.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Usage: ./ocr <image path> <network path (optional)>\n");
        return 1;
    }
    char *nn_path = DEFAULT_NN;
    if (argc >= 3)
    {
        nn_path = argv[2];
    }
}