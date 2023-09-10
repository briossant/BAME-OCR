#include "NeuralNetwork.h"
#include <stdio.h>

double WeightedSum(double *activations, Node node) {
    double sum = 0.0;
    for (size_t i = 0; i <node.weight_size; i++) {
        sum += activations[i] * node.weights[i];
    }
    sum -= node.bias;
    return sum;
}

double Sigmoid(double x) {
    return 1/ (1+exp(-x));
}

double *PropagateLayer(double *lastActivation, Layer layer) {
    double *newActiv = malloc(sizeof(double) * layer.size);
    for (size_t i = 0; i < layer.size; i++) {
        newActiv[i] = Sigmoid(WeightedSum(lastActivation, layer.nodes[i]));
    }
    return newActiv;
}

double* Propagate(double *inputs, Network network) {
    double *activations = inputs;
    for (size_t i = 0; i < network.depth;i++) {
        double *newActiv = PropagateLayer(activations, network.layers[i]);
        if(i>0) free(activations);
        activations = newActiv;
    }
    return activations;
}

