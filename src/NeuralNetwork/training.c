#include "NeuralNetwork.h"
#include <stddef.h>
#include <stdlib.h>

void shuffle(double **input, double** output, size_t n)
{
    for (size_t i = 0; i < n - 1; i++) 
    {
        size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
        double* t = input[j];
        input[j] = input[i];
        input[i] = t;
        t = output[j];
        output[j] = output[i];
        output[i] = t;
    }
}

InputBatch *MakeInputsBatch(double** inputs, double** outputs, 
        size_t nbr_of_batch, size_t batch_size) {
    InputBatch *res = malloc(sizeof(InputBatch)*nbr_of_batch);
    for (size_t i = 0; i<nbr_of_batch; i++){
        InputBatch batch = {
            .size = batch_size,
            .inputs = inputs+i*batch_size,
            .outputs = outputs+i*batch_size
        };
        res[i] = batch;
    }
    return res;
}

void TrainNetwork(Network network, double **inputs, double** outputs, 
        TrainingSettings settings)
{   
    size_t nbr_of_batch = settings.nbr_of_inputs/settings.batch_size;
    InputBatch *batchs = MakeInputsBatch(inputs, outputs, 
            nbr_of_batch, settings.batch_size);

    for (size_t i = 0; i < settings.epochs; i++) {
        shuffle(inputs, outputs, settings.nbr_of_inputs);
        double err = 0.0;
        for(size_t j=0;j<nbr_of_batch;j++)
            err += BackPropagation(network, settings.training_rate, batchs[j]);
        err /= nbr_of_batch;
        //if (i % (1 + epochs / 100) == 0);
        printf("~~{ [Training Epoch]: %ld -> [Accuracy]: %f }\n",i, err);
    }    
}
