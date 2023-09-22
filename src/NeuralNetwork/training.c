#include "NeuralNetwork.h"

void shuffle(double **input, double** output, size_t n)
{
    if (n > 1) 
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
}


InputBatch MakeInputBatch(double** inputs, double** outputs, size_t n) {
    InputBatch res = {
        .size = n,
        .inputs = inputs,
        .outputs = outputs
    };
    return res;
}


void trainNetwork(Network network, double **inputs, 
        double** outputs, size_t n, double trainingRate, 
        size_t epochs)
{    
    for (size_t i = 0; i < epochs; i++) {
        shuffle(inputs, outputs, n); 
        double err = BackPropagation(network, 
                trainingRate, MakeInputBatch(inputs, outputs, n));
        if (i % (1 + epochs / 100) == 0) {
            //printNetwork(network);
            printf("~~{ [Training Epoch]: %ld -> [Accuracy]: %f }\n",i, err);
        }
    }
    
    shuffle(inputs, outputs, n); 
}


