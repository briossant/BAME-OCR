#include "NeuralNetwork.h"


double dSigmoid(double x) {
    double sig = Sigmoid(x);
    return sig * (1 - sig);
}

void BackPropagation(Network network,  double **input_batch, size_t batch_size) {
     
}

