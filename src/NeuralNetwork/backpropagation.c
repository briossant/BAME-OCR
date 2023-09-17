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


void UpdateLayer(Layer layer, double* DCost) {
    for(size_t i=0;i<layer.size;i++) {
        layer.nodes[i].bias -= DCost[i] * TRAINING_RATE;
        for(size_t j=0;j<layer.nodes[i].weight_size;j++) {
            layer.nodes[i].weights[j] -= DCost[i] * TRAINING_RATE;    
        }    
    }
}


void BackPropagation(Network network,  double **input_batch, double **output_batch, size_t batch_size) {
    for (size_t m = 0; m < batch_size;m++) {
        double **activationsLayers = PropagateAndKeep(input_batch[m], network);
        
        size_t outputLayerI = network.depth-1;
        double *DCost = firstDCost(activationsLayers[outputLayerI], output_batch[m], network.layers[outputLayerI].size);
        
        // update first weights and biases
        UpdateLayer(network.layers[outputLayerI], DCost);

        for(size_t l = outputLayerI-1; l >= 0; l--) {
            
            double *newDCost = malloc(network.layers[l].size * sizeof(double));
            for (size_t i=0; i<network.layers[l].size;i++){
                // calculate newDCost 
                // start by calculating sig'(z[i])
                for (size_t j=0; j<network.layers[l+1].size;j++){
                    // newDCost[i] += (layer[j] -> weight[i]) * sig'(z[i]) * DCost[j]
                }
                
            }

            free(DCost);
            DCost = newDCost;

            // update layer l weights & biases according to new DCost
            UpdateLayer(network.layers[l], DCost);
        }

        free(DCost);
    }         
}

