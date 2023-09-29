#include "NeuralNetwork.h"
#include "activationFunctions.h"
#include <stddef.h>
#include <stdlib.h>



NNValue CostFunction(NNValue *outputActivations, NNValue *expectedOutputs, 
        size_t outputSize)
{
    NNValue res = 0;
    for(size_t i = 0; i < outputSize; i++){
        res += pow(outputActivations[i] - expectedOutputs[i], 2);
    }
    return res;
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


size_t MaxIndex(NNValue *list, size_t n) {
    size_t m = 0;
    NNValue max = list[0];
    for (size_t i=0;i<n;++i){
        if (list[i] > max) {
            m=i;
            max=list[i];
        }
    }
    return m;
}

NNValue IsRight(NNValue * activations, size_t size, NNValue* outputs){
    return MaxIndex(activations,  size) == MaxIndex(outputs, size);
}


NNValue TestPropagation(NNValue **inputs, NNValue **outputs, size_t nbr_of_inputs,
        Network network) 
{
    NNValue accuracy = 0.0;
    for(size_t i=0;i<nbr_of_inputs;i++)
        if (IsRight(Propagate(inputs[i], network),
                network.layers[network.depth-1].size, outputs[i])) 
            ++accuracy;
    return accuracy/nbr_of_inputs * 100;
}




