#include "matrices.h"
#include <stdio.h>
#include <stdlib.h>
#include <err.h>

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

void MatPrint(Matrix mat) {
    printf("~~ MATRIX (width: %zu | height: %zu) ~~ \n", mat.w, mat.h);
    for (size_t i = 0; i < mat.w; ++i) {
        printf("[%2zu]|", i);
        for (size_t j = 0; j < mat.h; j++) {
            printf(" %4lf ", mat.mat[i][j]);
        }
        printf("|\n");
    }
    printf("~~ MATRIX END ~~ \n");
}

Matrix MatInitWithFct(size_t w, size_t h, NNValue (*fct)()) {
    NNValue **list = malloc(sizeof(NNValue) * w);
    for (size_t i = 0; i < w; i++) {
        list[i] = malloc(sizeof(NNValue) * h);
        for (size_t j = 0; j < h; j++)
            list[i][j] = fct();
    }

    Matrix mat = {.w = w, .h = h, .mat = list};
    return mat;
}

Matrix MatCopy(Matrix mat) {
    NNValue **copy = malloc(sizeof(NNValue) * mat.w);
    for (size_t i = 0; i < mat.w; i++) {
        copy[i] = malloc(sizeof(NNValue) * mat.h);
        for (size_t j = 0; j < mat.h; j++)
            copy[i][j] = mat.mat[i][j];
    }

    Matrix mat_copy = {.w = mat.w, .h = mat.h, .mat = copy};
    return mat_copy;
}

void MatFree(Matrix mat) {
    for (size_t i = 0; i < mat.w; i++) {
        free(mat.mat[i]);
    }
    free(mat.mat);
    mat.w = 0;
    mat.h = 0;
}

Matrix MatGetVector(Matrix mat, size_t index) {
    NNValue **Vector = malloc(sizeof(NNValue *));
    Vector[0] = malloc(sizeof(NNValue) * mat.h);
    for (size_t i = 0; i < mat.h; i++) {
        Vector[0][i] = mat.mat[index][i];
    }
    Matrix vector = {.w = mat.w, .h = 1, .mat = Vector};
    return vector;
}

// made in place, matrix directly modified TODO: fix indexing
Matrix MatAddVector(Matrix mat, Matrix vect) {
    mat.w = mat.w + 1;
    mat.mat = (NNValue **)realloc(mat.mat, mat.w * sizeof(NNValue *));
    for (size_t i = 0; i < mat.w; i++) {
        mat.mat[i] = (NNValue *)realloc(mat.mat[i], mat.w * sizeof(NNValue));
        mat.mat[i][mat.w - 1] = vect.mat[i][0];
    }

    return mat;
}

// made in place, matrix directly modified
Matrix MatSetVector(Matrix mat, Matrix vect, size_t index) {
    if (mat.h != vect.h)
        errx(1, "MatSetVector: Matrices height should be the same");
    for (size_t i = 0; i < mat.h; i++) {
        mat.mat[index][i] = vect.mat[0][i];
    }

    return mat;
}

// TRUE->1 FALSE->0
Bool MatIsVector(Matrix mat) {
    if (mat.w == 1) {
        return True;
    }
    return False;
}
