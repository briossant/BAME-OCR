#include "matrices.h"

Matrix MatInit(size_t w, size_t h, NNValue defaultValue);

void MatPrint(Matrix mat);

Matrix MatInitWithFct(size_t w, size_t h, NNValue (*fct)());

Matrix MatCopy(Matrix mat);

void MatFree(Matrix mat);

Matrix MatGetVector(Matrix mat, size_t index);

Matrix MatSetVector(Matrix mat, Matrix vect, size_t index);

Bool MatIsVector(Matrix mat);
