#include "matrices.h"
#include <stdio.h>
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

void MatPrint(Matrix mat)
{
    printf(" Width: %zu \n", mat.w);
    printf(" Height: %zu \n", mat.h);
    for (size_t i=0; i<mat.w; ++i)
    { printf("|");
        for (size_t j=0; j<mat.h; j++)
        {
          printf ("%f", mat.mat[i][j]);  
        }
        printf("|\n");
    }
}

Matrix MatInitWithFct(size_t w, size_t h, NNValue (*fct)());

Matrix MatCopy(Matrix mat)
{
    NNValue **copy = malloc(sizeof(NNValue) * mat.w); 
    for (size_t i = 0; i < mat.w; i++) {
        copy[i] = malloc(sizeof(NNValue) *mat.h);
        for (size_t j = 0; j < mat.h; j++)
            copy[i][j] = mat.mat[i][j];
    }

    Matrix mat_copy = {.w = mat.w, .h = mat.h, .mat = copy};
    return mat_copy;
}

void MatFree(Matrix mat)
{
    for (size_t i=0; i<mat.h; i++)
    {
        free(mat.mat[i]);
    }
    free(mat.mat);
    mat.w=0;
    mat.h=0;
}

Matrix MatGetVector(Matrix mat, size_t index)
{ 
    NNValue **Vector = (NNValue **)calloc(mat.w, sizeof(NNValue *));
    for (size_t i=0; i<mat.w; i++)
    {  
        Vector[i] = malloc(sizeof(NNValue) *1);
        Vector[i][0]= mat.mat[mat.w][index];
    }
    Matrix vector = {.w = mat.w, .h =1, .mat = Vector};
    return vector;
}

// made in place, matrix directly modified
Matrix MatSetVector(Matrix mat, Matrix vect, size_t index)
{
    
    mat.w=mat.w+1; 
     mat.mat = (NNValue **)realloc(mat.mat, mat.w * sizeof(NNValue *));
    for (size_t i=0; i<mat.w; i++)
    {  
        mat.mat[i] = (NNValue *)realloc(mat.mat[i], mat.w * sizeof(NNValue));
        mat.mat[i][mat.w - 1] = vect.mat[i][0];
    }

    return mat;
}

// TRUE->1 FALSE->0
Bool MatIsVector(Matrix mat)
{
    if (mat.w==1){return 1;}
    return 0;
}
