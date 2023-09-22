#include "NeuralNetwork.h"


// sigmoid derivative with x = Sigmoid(...)
double DSigmoid(double x) {
    return x * (1 - x);
}


// cost function derivative over the output activations
double *FirstDCostDActv(double *outputActivations, double *expectedOutputs, 
        size_t outputSize) 
{
    double *res = malloc(outputSize * sizeof(double));
    for(size_t i = 0; i < outputSize; i++){
        res[i] = 2 * (outputActivations[i] - expectedOutputs[i]);
    }
    return res;
}


void UpdateLayer(Layer layer, double* dCost, double* lastActv) {
    for(size_t i=0;i<layer.size;i++) {
        layer.nodes[i].bias += dCost[i];
        for(size_t j=0;j<layer.nodes[i].weight_size;j++) {
            layer.nodes[i].weights[j] += dCost[i] * lastActv[j];    
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



double *DCostDZ(double* dCost_dActv, double* activations, size_t size) {
    double *dCost_dZ = malloc(sizeof(double) * size);
    for(size_t i = 0; i < size; i++) {
        dCost_dZ[i] = dCost_dActv[i] * DSigmoid(activations[i]);
    }
    return dCost_dZ;
}


double *GetNewDCostDActv(Layer layer, size_t size, double* activations, double* dCost_dActv) {
    double *new_dCost_dActv = malloc(size * sizeof(double));
    for (size_t i=0; i<size;i++){

        double dSig = DSigmoid(activations[i]);        
        new_dCost_dActv[i] = 0;
        for (size_t j=0; j<layer.size;j++)
        {
            new_dCost_dActv[i] += layer.nodes[j].weights[i] 
                    * dSig * dCost_dActv[j];
        }        
    }
    return new_dCost_dActv;
}



double** BackPropagateInput(Network network, double** activationsLayers, 
        double* outputs) {
    double **all_dCost_dZ = malloc(network.depth * sizeof(double*));
    size_t outputLayerI = network.depth-1;

    double *dCost_dActv = FirstDCostDActv(activationsLayers[outputLayerI], 
                outputs, network.layers[outputLayerI].size);

        
    all_dCost_dZ[outputLayerI] = DCostDZ(dCost_dActv, activationsLayers[outputLayerI], 
                    network.layers[outputLayerI].size);

    for(size_t l = outputLayerI; l > 0; l--) 
    {    
        double *new_dCost_dActv = GetNewDCostDActv(network.layers[l], 
                network.layers[l-1].size, activationsLayers[l], dCost_dActv);

        free(dCost_dActv);
        dCost_dActv = new_dCost_dActv;

        all_dCost_dZ[l-1] = DCostDZ(dCost_dActv, activationsLayers[l-1], 
                network.layers[l-1].size);
    }

    free(dCost_dActv);

    return all_dCost_dZ;
}



void AddToSumNetwork(Network sumNetwork, double** all_dCost_dZ, 
        double** activationsLayers, double* inputs) {

    UpdateLayer(sumNetwork.layers[0], all_dCost_dZ[0], inputs);
    for(size_t i = 1; i < sumNetwork.depth; i++) {
        UpdateLayer(sumNetwork.layers[i], all_dCost_dZ[i],
                activationsLayers[i-1]);
    }
        
    for(size_t i = 0; i < sumNetwork.depth; i++) {
        free(all_dCost_dZ[i]);
        free(activationsLayers[i]);
    }
    free(all_dCost_dZ);
    free(activationsLayers);
}


void UpdateNetwork(Network network, Network sumNetwork, double training_rate, size_t batch_size) {
    for(size_t l=0; l<network.depth;l++) {
        for(size_t i=0;i<network.layers[l].size;i++) {
            network.layers[l].nodes[i].bias -= sumNetwork.layers[l].nodes[i].bias / batch_size * training_rate;
            for(size_t j=0;j<network.layers[l].nodes[i].weight_size;j++) {
                network.layers[l].nodes[i].weights[j] -= sumNetwork.layers[l].nodes[i].weights[j] 
                    / batch_size * training_rate;
            }    
        }
    }
}


double BackPropagation(Network network, double training_rate, InputBatch batch) 
{ 
    double accuracy = 0;

    Network sumNetwork = copyAndResetNetwork(network);

    for (size_t m = 0; m < batch.size;m++) 
    {
        double **activationsLayers = PropagateAndKeep(batch.inputs[m], network);
        
        size_t outputLayerI = network.depth-1;        
        accuracy += CostFunction(activationsLayers[outputLayerI], batch.outputs[m],
                network.layers[outputLayerI].size);
    
        double** all_dCost_dZ = BackPropagateInput(network, activationsLayers, 
                batch.outputs[m]);
    
        AddToSumNetwork(sumNetwork, all_dCost_dZ, activationsLayers, batch.inputs[m]);
    }
    
    UpdateNetwork(network, sumNetwork, training_rate, batch.size); 

    //free sumNetwork

    return accuracy/batch.size;
}

