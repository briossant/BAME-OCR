#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include "NeuralNetwork.h"

#define NBR_HEADERS_IMAGES 4
#define NBR_HEADERS_LABELS 2

#define TRAINING_IMAGES_PATH "dataset/train-images.idx3-ubyte"
#define TESTING_IMAGES_PATH "dataset/t10k-images.idx3-ubyte"
#define TRAINING_LABELS_PATH "dataset/train-labels.idx1-ubyte"
#define TESTING_LABELS_PATH "dataset/t10k-labels.idx1-ubyte"

#define CHARS_IN_INT32 sizeof(int32_t)/sizeof(char)
#define NBR_OF_DIFFERENT_LABELS 10

#define IS_LABEL 1.0
#define IS_NOT_LABEL 0.0



int32_t *GetHeaders(FILE* file, size_t nbr) {
    unsigned char* buffer = malloc(nbr * CHARS_IN_INT32 * sizeof(char));
    fread(buffer, sizeof(char), nbr*CHARS_IN_INT32, file); 
    int32_t *headers  = malloc(sizeof(int32_t) * nbr);
    for(size_t i = 0; i < nbr; i++) {
        headers[i] = 0;
        for(size_t j = 0; j < CHARS_IN_INT32;j++){
            headers[i] <<= 8;
            headers[i] |= buffer[i*CHARS_IN_INT32+j];
        }
    }
    
    free(buffer);
    return headers;
}


double* GetImage(FILE* file, size_t image_size) {
    unsigned char *buffer =malloc(image_size * sizeof(unsigned char));
    fread(buffer, sizeof(char), image_size, file); 
    double* image = malloc(sizeof(double) * image_size);
    for(size_t i=0;i<image_size;i++)
        image[i] = (double)buffer[i] / 255;

    free(buffer);

    return image;
}

double **LoadMnistImages(char* path) 
{
    FILE *file = fopen(path, "r");
    if (file == NULL) {
        errx(1, "loadMnistImages: unable to open dataset file (%s)", path);
    }

    // 0: magic number ; 1: number of images ; 2: image height ; 3: image width
    int32_t *headers  = GetHeaders(file, NBR_HEADERS_IMAGES);   

    printf("Loading images (magic: %d) -> nbr: %d  h: %d  w: %d\n", headers[0], headers[1], headers[2], headers[3]);

    double **images = malloc(sizeof(double*) * headers[1]);
    size_t image_size = headers[2] * headers[3];
    for (int i = 0; i < headers[1];i++)
        images[i] = GetImage(file, image_size);

    return images;
}

double **LoadMnistLabels(char* path) {
    FILE *file = fopen(path, "r");
    if (file == NULL) {
        errx(1, "loadMnistLabels: unable to open dataset file (%s)", path);
    }

    // 0: magic number ; 1: number of labels ;
    int32_t *headers  = GetHeaders(file, NBR_HEADERS_LABELS);   

    printf("Loading labels (magic: %d) -> nbr: %d\n", headers[0], headers[1]);

     
    unsigned char *buffer =malloc(headers[1] * sizeof(unsigned char));
    fread(buffer, sizeof(char), headers[1], file); 
    
    double **labels = malloc(sizeof(double*) * headers[1]);
    for (int i = 0; i < headers[1];i++){
        labels[i] = malloc(sizeof(double) * NBR_OF_DIFFERENT_LABELS);
        for (unsigned char j = 0; j < NBR_OF_DIFFERENT_LABELS; j++){
            if (buffer[i] == j) 
                labels[i][j] = IS_LABEL;
            else
                labels[i][j] = IS_NOT_LABEL;
        }
    }
    
    free(buffer);

    return labels;
}


void LoadMnist(double*** images, double*** labels, Bool isForTraining) {
    *images = LoadMnistImages(isForTraining ? TRAINING_IMAGES_PATH : TESTING_IMAGES_PATH);
    *labels = LoadMnistLabels(isForTraining ? TRAINING_LABELS_PATH : TESTING_LABELS_PATH);
}



