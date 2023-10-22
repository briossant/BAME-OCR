#include "NeuralNetwork.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#define NBR_HEADERS_IMAGES 4
#define NBR_HEADERS_LABELS 2

#define TRAINING_IMAGES_PATH "dataset/nbr60k.img"
#define TESTING_IMAGES_PATH "dataset/nbr10k.img"
#define TRAINING_LABELS_PATH "dataset/nbr60k.label"
#define TESTING_LABELS_PATH "dataset/nbr10k.label"

#define CHARS_IN_INT32 sizeof(int32_t) / sizeof(char)
#define NBR_OF_DIFFERENT_LABELS 10

#define IS_LABEL 1.0
#define IS_NOT_LABEL 0.0

int32_t *GetHeaders(FILE *file, size_t nbr) {
    unsigned char *buffer = malloc(nbr * CHARS_IN_INT32 * sizeof(char));
    fread(buffer, sizeof(char), nbr * CHARS_IN_INT32, file);
    int32_t *headers = malloc(sizeof(int32_t) * nbr);
    for (size_t i = 0; i < nbr; i++) {
        headers[i] = 0;
        for (size_t j = 0; j < CHARS_IN_INT32; j++) {
            headers[i] <<= 8;
            headers[i] |= buffer[i * CHARS_IN_INT32 + j];
        }
    }

    free(buffer);
    return headers;
}

Matrix GetImage(FILE *file, size_t image_size) {
    unsigned char *buffer = malloc(image_size * sizeof(unsigned char));
    fread(buffer, sizeof(char), image_size, file);
    Matrix image = MatInit(1, image_size, 0, "Mnist image");
    for (size_t i = 0; i < image_size; i++)
        image.mat[0][i] = (NNValue)buffer[i] / 255;

    free(buffer);

    return image;
}

Matrix LoadMnistImages(char *path) {
    FILE *file = fopen(path, "r");
    if (file == NULL) {
        errx(1,
             "loadMnistImages: unable to open dataset file (%s), did you run "
             "downLoadMnist.sh?",
             path);
    }

    // 0: magic number ; 1: number of images ; 2: image height ; 3: image width
    int32_t *headers = GetHeaders(file, NBR_HEADERS_IMAGES);

    printf("Loading images (magic: %d) -> nbr: %d  h: %d  w: %d\n", headers[0],
           headers[1], headers[2], headers[3]);

    Matrix images =
        MatInit(headers[1], headers[2] * headers[3], 0, "Mnist images");
    size_t image_size = headers[2] * headers[3];
    for (int i = 0; i < headers[1]; i++) {
        Matrix img = GetImage(file, image_size);
        MatSetVector(images, img, i);
        MatFree(img);
    }
    fclose(file);

    return images;
}

Matrix LoadMnistLabels(char *path) {
    FILE *file = fopen(path, "r");
    if (file == NULL) {
        errx(1,
             "loadMnistLabels: unable to open dataset file (%s), did you run "
             "downLoadMnist.sh?",
             path);
    }

    // 0: magic number ; 1: number of labels ;
    int32_t *headers = GetHeaders(file, NBR_HEADERS_LABELS);

    printf("Loading labels (magic: %d) -> nbr: %d\n", headers[0], headers[1]);

    unsigned char *buffer = malloc(headers[1] * sizeof(unsigned char));
    fread(buffer, sizeof(char), headers[1], file);

    Matrix labels = MatInit(headers[1], NBR_OF_DIFFERENT_LABELS, IS_NOT_LABEL,
                            "Mnist labels");
    for (int i = 0; i < headers[1]; i++) {
        labels.mat[i][buffer[i]] = IS_LABEL;
    }

    free(buffer);
    fclose(file);

    return labels;
}

void LoadMnist(Matrix *images, Matrix *labels, Bool isForTraining) {
    printf("|||||||||||||||||| Loading Mnist ||||||||||||||||||||||\n");
    *images = LoadMnistImages(isForTraining ? TRAINING_IMAGES_PATH
                                            : TESTING_IMAGES_PATH);
    *labels = LoadMnistLabels(isForTraining ? TRAINING_LABELS_PATH
                                            : TESTING_LABELS_PATH);
    printf("|||||||||||||||||| Mnist Loaded |||||||||||||||||||||||\n\n");
}
