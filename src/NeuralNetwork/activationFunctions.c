#include "activationFunctions.h"


/////////////////////////////////////
/// ZFunctions


NNValue WeightedSum(NNValue *activations, Node node) {
    NNValue sum = 0.0;
    for (size_t i = 0; i <node.weight_size; i++) {
        sum += activations[i] * node.weights[i];
    }
    sum -= node.bias;
    return sum;
}


/////////////////////////////////////
/// SquishingFunctions


NNValue Sigmoid(NNValue x) {
    return 1/ (1+exp(-x));
}
// sigmoid derivative with x = Sigmoid(...)
NNValue DSigmoid(NNValue x) {
    return x * (1 - x);
}


