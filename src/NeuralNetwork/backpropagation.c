#include "NeuralNetwork.h"

#define TRAINING_RATE 3.0

// sigmoid derivative
double dSigmoid(double x) {
    double sig = Sigmoid(x);
    return sig * (1 - sig);
}


// cost function derivative
double DCost(double result, double expected) {
    return 2 * (result - expected);    
}


// cost dFunc for all outputs 
double *firstDCost(double *outputActivations, double *expectedOutputs, size_t outputSize) {
    double *res = malloc(outputSize * sizeof(double));
    for(size_t i = 0; i < outputSize; i++){
        res[i] = DCost(outputActivations[i], expectedOutputs[i]);
    }
    return res;
}


void BackPropagation(Network network,  double **input_batch, double **output_batch, size_t batch_size) {
    for (size_t m = 0; m < batch_size;m++) {
        double **activationsLayers = PropagateAndKeep(input_batch[m], network);
        
        size_t outputLayerI = network.depth-1;
        double *lastDCost = firstDCost(activationsLayers[outputLayerI], output_batch[m], network.layers[outputLayerI].size);
        
        for(size_t l = outputLayerI; l >= 0; l--) {
            // update weight & bias according to lastDCost
            // calculate new DCost
        }

    }         
}

