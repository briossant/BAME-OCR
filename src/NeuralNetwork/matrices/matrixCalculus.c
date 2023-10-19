#include "matrices.h"

void CheckIfSameMatDim(Matrix matA, Matrix matB) {
    if (matA.h != matB.h || matA.w != matB.w){
        printf("EEEEEEEEEERRRRRRRRRRRRRRRRRROOOOOOOOOOOOOOOOOOOOOORRRRRRRRRRRRRRRRRRR\n");
        MatPrint(matA);
        MatPrint(matB);
        errx(0, "Matrix A and B must have the same dimensions");
    }
}

// add term by term, matrices should have the same size
Matrix MatAdd(Matrix matA, Matrix matB) {
    CheckIfSameMatDim(matA, matB);
    for (size_t i = 0; i < matA.w; i++)
        for (size_t j = 0; j < matB.h; j++)
            matA.mat[i][j] += matB.mat[i][j];
    return matA;
}

// substract term by term, matrices should have the same size
Matrix MatSub(Matrix matA, Matrix matB) {
    CheckIfSameMatDim(matA, matB);
    for (size_t i = 0; i < matA.w; i++)
        for (size_t j = 0; j < matB.h; j++)
            matA.mat[i][j] -= matB.mat[i][j];
    return matA;
}

// matrix dot product
Matrix MatDot(Matrix matA, Matrix matB){
    if (matA.w != matB.h) {
        printf("EEEEEEEEEERRRRRRRRRRRRRRRRRROOOOOOOOOOOOOOOOOOOOOORRRRRRRRRRRRRRRRRRR\n");
        MatPrint(matA);
        MatPrint(matB);
        errx(0, "MatDot: matrix size aren't right");
    }
    char* label;
    asprintf(&label, "Dot of [%s] * [%s]", matA.label, matB.label);
    Matrix res = MatInit(matB.w, matA.h, 0, label);
    for (size_t i = 0; i < matB.w; i++)
        for (size_t j = 0; j < matA.h; j++)
            for (size_t h = 0; h < matB.h; h++) 
                res.mat[i][j] += matA.mat[h][j] * matB.mat[i][h];
    return res;
}

// multiply term by term, matrices should have the same size
Matrix MatMult(Matrix matA, Matrix matB) {
    CheckIfSameMatDim(matA, matB);
    for (size_t i = 0; i < matA.w; i++)
        for (size_t j = 0; j < matB.h; j++)
            matA.mat[i][j] *= matB.mat[i][j];
    return matA;
}

Matrix MatAddScalar(Matrix mat, NNValue scalar) {
    for (size_t i = 0; i < mat.w; i++)
        for (size_t j = 0; j < mat.h; j++)
            mat.mat[i][j] += scalar;
    return mat;
}

Matrix MatMultScalar(Matrix mat, NNValue scalar) {
    for (size_t i = 0; i < mat.w; i++)
        for (size_t j = 0; j < mat.h; j++)
            mat.mat[i][j] *= scalar;
    return mat;
}

Matrix MatApplyFct(Matrix mat, MatFct matFct) {
    for (size_t i = 0; i < mat.w; i++)
        for (size_t j = 0; j < mat.h; j++)
            mat.mat[i][j] = matFct(mat.mat[i][j]);
    return mat;
}

// sum of all the terms
NNValue MatSum(Matrix mat){
    NNValue sum = 0.0;
    for (size_t i = 0; i < mat.w; i++)
        for (size_t j = 0; j < mat.h; j++)
            sum += mat.mat[i][j]; 
    return sum;
}
