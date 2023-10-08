#include "NeuralNetwork.h"
#include <stdio.h>

// sigmoid derivative with x = Sigmoid(...)
NNValue DSigmoid(NNValue x) { return x * (1 - x); }

// cost function derivative over the output activations
Matrix FirstDCostDActv(Matrix outputActivations, Matrix expectedOutputs) {
    return MatMultScalar(MatSub(MatCopy(outputActivations), expectedOutputs),
                         2);
}

void UpdateLayer(Layer layer, Matrix dCost, Matrix lastActv) {
    MatAdd(MatCopy(layer.biases), dCost);
    MatAdd(MatCopy(layer.weights), MatMult(dCost, lastActv));
}

Matrix DCostDZ(Matrix dCost_dActv, Matrix activations) {
    return MatMult(MatApplyFct(MatCopy(activations), DSigmoid), dCost_dActv);
}

Matrix GetNewDCostDActv(Layer layer, Matrix activations, Matrix dCost_dActv) {
    return MatMult(MatDot(layer.weights, dCost_dActv),
                   MatApplyFct(MatCopy(activations), DSigmoid));
}

Matrix *BackPropagateInput(Network network, Matrix *activationsLayers,
                           Matrix outputs) {
    Matrix *all_dCost_dZ = malloc(network.depth * sizeof(Matrix));
    size_t outputLayerI = network.depth - 1;

    Matrix dCost_dActv =
        FirstDCostDActv(activationsLayers[outputLayerI], outputs);

    all_dCost_dZ[outputLayerI] =
        DCostDZ(dCost_dActv, activationsLayers[outputLayerI]);

    for (size_t l = outputLayerI; l > 0; l--) {
        Matrix new_dCost_dActv = GetNewDCostDActv(
            network.layers[l], activationsLayers[l], dCost_dActv);

        MatFree(dCost_dActv);
        dCost_dActv = new_dCost_dActv;

        all_dCost_dZ[l - 1] = DCostDZ(dCost_dActv, activationsLayers[l - 1]);
    }

    MatFree(dCost_dActv);

    return all_dCost_dZ;
}

void AddToSumNetwork(Network sumNetwork, Matrix *all_dCost_dZ,
                     Matrix *activationsLayers, Matrix inputs) {

    UpdateLayer(sumNetwork.layers[0], all_dCost_dZ[0], inputs);
    MatFree(all_dCost_dZ[0]);
    MatFree(activationsLayers[0]);
    for (size_t i = 1; i < sumNetwork.depth; i++) {
        UpdateLayer(sumNetwork.layers[i], all_dCost_dZ[i],
                    activationsLayers[i - 1]);
        MatFree(all_dCost_dZ[i]);
        MatFree(activationsLayers[i]);
    }
    free(activationsLayers);
    free(all_dCost_dZ);
}

// watchout sumValue and inertiaValue are modified by the function
Matrix ValueToRemove(Matrix sumValue, Matrix inertiaValue,
                     TrainingSettings settings, size_t batch_size) {

    NNValue scalar = -(1.0 - settings.inertia_strength) / batch_size *
                     settings.training_rate;
    return MatAdd(MatMultScalar(sumValue, scalar),
                  MatMultScalar(inertiaValue, settings.inertia_strength));
}

void UpdateNetwork(Network network, Network sumNetwork, Network inertiaNetwork,
                   TrainingSettings settings, size_t batch_size) {
    for (size_t l = 0; l < network.depth; l++) {

        Layer layer = network.layers[l];
        Layer sumLayer = sumNetwork.layers[l];
        Layer inertiaLayer = inertiaNetwork.layers[l];

        // update biases
        Matrix bdif = ValueToRemove(sumLayer.biases, inertiaLayer.biases,
                                    settings, batch_size);
        MatAdd(layer.biases, bdif);
        inertiaLayer.biases = MatMultScalar(bdif, -1);

        // update weights
        Matrix wdif = ValueToRemove(sumLayer.weights, inertiaLayer.weights,
                                    settings, batch_size);
        MatAdd(layer.weights, wdif);
        inertiaLayer.weights = MatMultScalar(wdif, -1);

        MatFree(bdif);
        MatFree(wdif);
    }
}

NNValue BackPropagation(Network network, TrainingSettings settings,
                        InputBatch batch, Network inertiaNetwork) {
    NNValue accuracy = 0;

    Network sumNetwork = copyAndResetNetwork(network);

    for (size_t m = 0; m < batch.size; m++) {

        Matrix inputs = MatGetVector(batch.inputs, m);
        Matrix outputs = MatGetVector(batch.outputs, m);

        Matrix *activationsLayers = PropagateAndKeep(inputs, network);

        size_t outputLayerI = network.depth - 1;
        accuracy += CostFunction(activationsLayers[outputLayerI], outputs);

        Matrix *all_dCost_dZ =
            BackPropagateInput(network, activationsLayers, outputs);

        AddToSumNetwork(sumNetwork, all_dCost_dZ, activationsLayers, inputs);
    }

    UpdateNetwork(network, sumNetwork, inertiaNetwork, settings, batch.size);

    freeNetwork(sumNetwork);

    return accuracy / batch.size;
}
