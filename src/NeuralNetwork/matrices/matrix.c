#include "matrices.h"
#include <stdlib.h>

Matrix MatInit(size_t w, size_t h, NNValue defaultValue) {
    NNValue **list = malloc(sizeof(NNValue) * w);
    for (size_t i = 0; i < w; i++) {
        list[i] = malloc(sizeof(NNValue) * h);
        for (size_t j = 0; j < h; j++)
            list[i][j] = defaultValue;
    }

    Matrix mat = {.w = w, .h = h, .mat = list};
    return mat;
}

void MatPrint(Matrix mat);

Matrix MatInitWithFct(size_t w, size_t h, NNValue (*fct)());

Matrix MatCopy(Matrix mat);

void MatFree(Matrix mat);

Matrix MatGetVector(Matrix mat, size_t index);

Matrix MatSetVector(Matrix mat, Matrix vect, size_t index);

Bool MatIsVector(Matrix mat);
