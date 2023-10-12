#include "matrices.h"
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
