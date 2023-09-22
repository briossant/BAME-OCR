#include <stddef.h>
#include <stdint.h>
#include "NeuralNetwork.h"

#define NBR_HEADERS 4
#define TRAINING_IMAGES_PATH "dataset/train-images-idx3-ubyte.gz"


double **loadMnistImages() 
{
    FILE *file = fopen(TRAINING_IMAGES_PATH, "r");
    
    if (file == NULL) {
        errx(1, "loadMnistImages: unable to open dataset file (%s)", TRAINING_IMAGES_PATH);
    }

    // 0: magic number ; 1: number of images ; 2: image height ; 3: image width
    int32_t *headers  = malloc(sizeof(int32_t) * NBR_HEADERS);   
    fread(headers, sizeof(int32_t), NBR_HEADERS, file); 

    double **images = malloc(sizeof(double*) * headers[1]);

    size_t image_size = headers[2] * headers[3];

    for(int32_t i = 0; i < headers[1];i++) {
        double* image = malloc(sizeof(double) * image_size);
        fread(image, sizeof(double), image_size, file);
        images[i] = image;
    }

    return images;
}



