#include "NeuralNetwork.h"

#define TRAINING_RATE 3.0

// sigmoid derivative with x = Sigmoid(...)
double DSigmoid(double x) {
    return x * (1 - x);
}


// cost function derivative over an output activation
double GetDCost(double result, double expected) {
    return 2 * (result - expected);    
}


// cost function derivative over the output activations
double *FirstDCost(double *outputActivations, double *expectedOutputs, 
        size_t outputSize) 
{
    double *res = malloc(outputSize * sizeof(double));
    for(size_t i = 0; i < outputSize; i++){
        res[i] = GetDCost(outputActivations[i], expectedOutputs[i]);
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

double CostFunction(double *outputActivations, double *expectedOutputs, 
        size_t outputSize)
{
    double res = 0;
    for(size_t i = 0; i < outputSize; i++){
        res += pow(outputActivations[i] - expectedOutputs[i], 2);
    }
    return res;
}


double BackPropagation(Network network,  double **input_batch, 
        double **output_batch, size_t batch_size) 
{
    double accuracy = 0;

    for (size_t m = 0; m < batch_size;m++) 
    {
        double **activationsLayers = PropagateAndKeep(input_batch[m], network);
        
        size_t outputLayerI = network.depth-1;
        
        accuracy += CostFunction(activationsLayers[outputLayerI], output_batch[m],
                network.layers[outputLayerI].size);
        double *dCost = FirstDCost(activationsLayers[outputLayerI], 
                output_batch[m], network.layers[outputLayerI].size);

        
        // update first weights and biases
        UpdateLayer(network.layers[outputLayerI], dCost);

        for(size_t l = outputLayerI; l > 0; l--) 
        {    
            double *newDCost = malloc(network.layers[l-1].size * sizeof(double));
            for (size_t i=0; i<network.layers[l-1].size;i++){
                
                // calculate newDCost 
                
                // start by calculating sig'(z[i])
                double dSig = DSigmoid(activationsLayers[l][i]);
                
                newDCost[i] = 0;
                for (size_t j=0; j<network.layers[l].size;j++)
                {
                    newDCost[i] += network.layers[l].nodes[j].weights[i] 
                        * dSig * dCost[j];
                }
                
            }

            free(dCost);
            dCost = newDCost;

            // update layer l weights & biases according to new DCost
            UpdateLayer(network.layers[l-1], dCost);
        }

        free(dCost);
    }

    return accuracy/batch_size;
}

