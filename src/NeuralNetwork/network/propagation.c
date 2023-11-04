#include "NeuralNetwork.h"
#include "err.h"
#include <stddef.h>
#include <stdlib.h>

Matrix WeightedSum(Matrix activations, Layer layer) {
    return MatDot(layer.weights, activations);
}

NNValue Pow2(NNValue x) { return pow(x, 2); }

NNValue CostFunction(Matrix outputActivations, Matrix expectedOutputs) {
    Matrix mat = MatApplyFct(
        MatSub(MatCopy(expectedOutputs, "CostFunction"), outputActivations),
        Pow2);
    NNValue sum = MatSum(mat);
    MatFree(mat);
    return sum;
}

NNValue Sigmoid(NNValue x) { return 1 / (1 + exp(-x)); }

Matrix PropagateLayer(Matrix lastActivation, Layer layer) {
    return MatApplyFct(WeightedSum(lastActivation, layer), Sigmoid);
}

Matrix Propagate(Matrix inputs, Network network) {
    Matrix activations = inputs;
    for (size_t i = 0; i < network.depth; i++) {
        Matrix newActiv = PropagateLayer(activations, network.layers[i]);
        if (i > 0)
            MatFree(activations);
        activations = newActiv;
    }
    return activations;
}

Matrix *PropagateAndKeep(Matrix inputs, Network network) {
    Matrix *activationsLayers = malloc(network.depth * sizeof(Matrix));
    Matrix lastActivations = inputs;
    for (size_t i = 0; i < network.depth; i++) {
        lastActivations = PropagateLayer(lastActivations, network.layers[i]);
        activationsLayers[i] = lastActivations;
    }
    return activationsLayers;
}

size_t VectorMaxIndex(Matrix mat) {
    if (!MatIsVector(mat))
        errx(1, "VectorMaxIndex: matrix is not a vector");
    size_t m = 0;
    NNValue max = mat.mat[0][0];
    for (size_t i = 0; i < mat.h; ++i) {
        if (mat.mat[0][i] > max) {
            m = i;
            max = mat.mat[0][i];
        }
    }
    return m;
}

NNValue IsRight(Matrix activations, Matrix outputs) {
    return VectorMaxIndex(activations) == VectorMaxIndex(outputs);
}

NNValue TestPropagation(Matrix inputs, Matrix outputs, Network network) {
    NNValue accuracy = 0.0;
    for (size_t i = 0; i < inputs.w; i++)
        if (IsRight(Propagate(MatGetVector(inputs, i), network),
                    MatGetVector(outputs, i)))
            ++accuracy;
    return accuracy / inputs.w * 100;
}
