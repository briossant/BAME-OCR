#ifndef MATRICES_H
#define MATRICES_H

#include <stddef.h>

#include "../precision.h"


typedef struct Matrix {
    size_t w;
    size_t h;
    NNValue** mat;
} Matrix;


Matrix MatInit(size_t w, size_t h, NNValue defaultValue);

Matrix MatCopy(Matrix mat);

// don't perform calculus in place, the matrix should remained unchanged.
Matrix MatAdd(Matrix matA, Matrix matB);

Matrix MatMult(Matrix matA, Matrix matB);

Matrix MatAddScalar(Matrix mat, NNValue scalar);

Matrix MatMultScalar(Matrix mat, NNValue scalar);


#endif
