#ifndef MATRICES_H
#define MATRICES_H

#include <stddef.h>

#include "../precision.h"

typedef struct Matrix {
    size_t w;
    size_t h;
    NNValue **mat;
} Matrix;

typedef NNValue (*MatFct)(NNValue x);

Matrix MatInit(size_t w, size_t h, NNValue defaultValue);

Matrix MatInitWithFct(size_t w, size_t h, NNValue (*fct)());

Matrix MatCopy(Matrix mat);

void MatFree(Matrix mat);

Matrix MatGetVector(Matrix mat, size_t index);

Matrix MatSetVector(Matrix mat, Matrix vect, size_t index);

Bool MatIsVector(Matrix mat);

// perform calculus in place for matA, the matB should remained unchanged.

// add term by term, matrices should have the same size
Matrix MatAdd(Matrix matA, Matrix matB);

// substract term by term, matrices should have the same size
Matrix MatSub(Matrix matA, Matrix matB);

// matrix dot product
Matrix MatDot(Matrix matA, Matrix matB);

// multiply term by term, matrices should have the same size
Matrix MatMult(Matrix matA, Matrix matB);

Matrix MatAddScalar(Matrix mat, NNValue scalar);

Matrix MatMultScalar(Matrix mat, NNValue scalar);

Matrix MatApplyFct(Matrix mat, MatFct matFct);

// sum of all the terms
NNValue MatSum(Matrix mat);

#endif
