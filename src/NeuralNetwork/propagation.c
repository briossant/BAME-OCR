#include "NeuralNetwork.h"
#include <stdlib.h>

NNValue WeightedSum(NNValue *activations, Node node) {
    NNValue sum = 0.0;
    for (size_t i = 0; i <node.weight_size; i++) {
        sum += activations[i] * node.weights[i];
    }
    sum -= node.bias;
    return sum;
}


NNValue CostFunction(NNValue *outputActivations, NNValue *expectedOutputs, 
        size_t outputSize)
{
    NNValue res = 0;
    for(size_t i = 0; i < outputSize; i++){
        res += pow(outputActivations[i] - expectedOutputs[i], 2);
    }
    return res;
}


NNValue Sigmoid(NNValue x) {
    return 1/ (1+exp(-x));
}

NNValue *PropagateLayer(NNValue *lastActivation, Layer layer) {
    NNValue *newActiv = malloc(sizeof(NNValue) * layer.size);
    for (size_t i = 0; i < layer.size; i++) {
        newActiv[i] = Sigmoid(WeightedSum(lastActivation, layer.nodes[i]));
    }
    return newActiv;
}

NNValue* Propagate(NNValue *inputs, Network network) {
    NNValue *activations = inputs;
    for (size_t i = 0; i < network.depth;i++) {
        NNValue *newActiv = PropagateLayer(activations, network.layers[i]);
        if(i>0) free(activations);
        activations = newActiv;
    }
    return activations;
}


NNValue **PropagateAndKeep(NNValue *inputs, Network network) {
    NNValue **activationsLayers = malloc(sizeof(NNValue *) * network.depth);
    NNValue *lastActivations = inputs;
    for (size_t i = 0; i < network.depth;i++) {
        lastActivations = PropagateLayer(lastActivations, network.layers[i]);
        activationsLayers[i] = lastActivations;
    }
    return activationsLayers;
}


NNValue TestPropagation(NNValue **inputs, NNValue **outputs, size_t nbr_of_inputs,
        Network network) 
{
    NNValue accuracy = 0.0;
    for(size_t i=0;i<nbr_of_inputs;i++)
        accuracy += CostFunction(Propagate(inputs[i], network), outputs[i],
                network.layers[network.depth-1].size);
    return accuracy/nbr_of_inputs;
}




