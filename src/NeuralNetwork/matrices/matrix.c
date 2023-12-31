#include "matrices.h"

Matrix MatInit(size_t w, size_t h, NNValue defaultValue, char *label) {
    NNValue **list = malloc(sizeof(NNValue *) * w);
    for (size_t i = 0; i < w; i++) {
        list[i] = malloc(sizeof(NNValue) * h);
        for (size_t j = 0; j < h; j++)
            list[i][j] = defaultValue;
    }

    Matrix mat = {.w = w, .h = h, .mat = list, .label = label};
    return mat;
}

void MatPrint(Matrix mat) {
    printf("\n~~ [%s] MATRIX (width: %zu | height: %zu) ~~ \n", mat.label,
           mat.w, mat.h);
    for (size_t i = 0; i < mat.w; ++i) {
        printf("[%2zu]|", i);
        for (size_t j = 0; j < mat.h; j++) {
            printf(" %4lf ", mat.mat[i][j]);
        }
        printf("|\n");
    }
    printf("~~ [%s] MATRIX END ~~ \n\n", mat.label);
}

Matrix MatInitWithFct(size_t w, size_t h, NNValue (*fct)(), char *label) {
    NNValue **list = malloc(sizeof(NNValue *) * w);
    for (size_t i = 0; i < w; i++) {
        list[i] = malloc(sizeof(NNValue) * h);
        for (size_t j = 0; j < h; j++)
            list[i][j] = fct();
    }

    Matrix mat = {.w = w, .h = h, .mat = list, .label = label};
    return mat;
}

Matrix MatCopy(Matrix mat, char *label) {
    NNValue **copy = malloc(sizeof(NNValue *) * mat.w);
    for (size_t i = 0; i < mat.w; i++) {
        copy[i] = malloc(sizeof(NNValue) * mat.h);
        for (size_t j = 0; j < mat.h; j++)
            copy[i][j] = mat.mat[i][j];
    }

    Matrix mat_copy = {.w = mat.w, .h = mat.h, .mat = copy, .label = label};
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
    char *label;
    if (LABEL_TRANSFORM)
        asprintf(&label, "Vector of %s", mat.label);
    else
        label = "vector";
    Matrix vector = {.w = 1, .h = mat.h, .mat = Vector, .label = label};
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

NNValue *matrix_to_array(Matrix mat) {
    NNValue *array = (NNValue *)malloc(sizeof(NNValue) * (mat.h * mat.w));
    for (size_t i = 0; i < mat.w; ++i) {
        for (size_t j = 0; j < mat.h; j++) {
            array[i * mat.h + j] = mat.mat[i][j];
        }
    }
    return array;
}

Matrix array_to_matrix(double *array, size_t w, size_t h) {
    Matrix copy = MatInit(w, h, 0, "FromLoad");
    int k = 0;
    for (size_t i = 0; i < w; i++) {
        for (size_t j = 0; j < h; j++) {
            copy.mat[i][j] = array[k];
            k++;
        }
    }

    return copy;
}

Matrix image_to_matrix(SDL_Surface *image) {
    Matrix matimg = MatInit(1, image->w * image->h, 0, "img to matrx");

    const SDL_PixelFormat *format = image->format;
    Uint32 *pixtab = image->pixels;
    Uint8 r, g, b, a;
    double val;
    for (int x = 0; x < image->w; ++x) {
        for (int y = 0; y < image->h; ++y) {
            SDL_GetRGBA(pixtab[y * image->w + x], format, &r, &g, &b, &a);
            val = r;
            matimg.mat[0][y * image->w + x] = (val / 255);
        }
    }

    return matimg;
}
