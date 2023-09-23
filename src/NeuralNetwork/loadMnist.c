#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include "NeuralNetwork.h"

#define NBR_HEADERS 4
#define TRAINING_IMAGES_PATH "dataset/train-images.idx3-ubyte"
#define CHARS_IN_INT32 sizeof(int32_t)/sizeof(char)

int32_t *getHeaders(FILE* file, size_t nbr) {
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


double* getImage(FILE* file, size_t image_size) {
    unsigned char *buffer =malloc(image_size * sizeof(unsigned char));
    fread(buffer, sizeof(char), image_size, file); 
    double* image = malloc(sizeof(double) * image_size);
    for(size_t i=0;i<image_size;i++)
        image[i] = (double)buffer[i] / 255;

    return image;
}

double **loadMnistImages() 
{
    FILE *file = fopen(TRAINING_IMAGES_PATH, "r");
    if (file == NULL) {
        errx(1, "loadMnistImages: unable to open dataset file (%s)",
                TRAINING_IMAGES_PATH);
    }

    // 0: magic number ; 1: number of images ; 2: image height ; 3: image width
    int32_t *headers  = getHeaders(file, NBR_HEADERS);   

    printf("plz: %d %d %d %d\n", headers[0], headers[1], headers[2], headers[3]);

    double **images = malloc(sizeof(double*) * headers[1]);
    size_t image_size = headers[2] * headers[3];
    for (int i = 0; i < headers[1];i++)
        images[i] = getImage(file, image_size);

    return images;
}



