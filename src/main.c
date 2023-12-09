#include "ImageProcessing/ImageProcess.h"
#include "Interface/interface.h"
#include "NeuralNetwork/network/NeuralNetwork.h"
#include "SudokuSolver/Sudoku_Solver.h"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_surface.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: ./BAME <image path>\n");
        return 1;
    }
    ThreadParameters *parameters = malloc(sizeof(ThreadParameters));
    parameters->angle = 0;
    parameters->auto_rotate = 1;
    parameters->step_index = 42;
    parameters->filename = argv[1];
    parameters->filename_resolved = "BAME.png";
    BAME(parameters);
    return 0;
}
