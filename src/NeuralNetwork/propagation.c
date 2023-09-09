#include "NeuralNetwork.h"
#include <math.h>

double WeightedSum(double *activations, Node node) {
    double sum = 0.0;
    for (int i = 0; i <node.weight_size; i++) {
        sum += activations[i] * node.weights[i];
    }
    sum -= node.bias;
    return sum;
}

double Sigmoid(double x) {
    return 1/ (1-exp(-x));
}

double* Propagate(double *inputs, Network network);

