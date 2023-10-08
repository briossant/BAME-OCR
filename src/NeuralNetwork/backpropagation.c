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

NNValue **BackPropagateInput(Network network, NNValue **activationsLayers,
                             NNValue *outputs) {
    NNValue **all_dCost_dZ = malloc(network.depth * sizeof(NNValue *));
    size_t outputLayerI = network.depth - 1;

    NNValue *dCost_dActv =
        FirstDCostDActv(activationsLayers[outputLayerI], outputs,
                        network.layers[outputLayerI].size);

    all_dCost_dZ[outputLayerI] =
        DCostDZ(dCost_dActv, activationsLayers[outputLayerI],
                network.layers[outputLayerI].size);

    for (size_t l = outputLayerI; l > 0; l--) {
        NNValue *new_dCost_dActv =
            GetNewDCostDActv(network.layers[l], network.layers[l - 1].size,
                             activationsLayers[l], dCost_dActv);

        free(dCost_dActv);
        dCost_dActv = new_dCost_dActv;

        all_dCost_dZ[l - 1] = DCostDZ(dCost_dActv, activationsLayers[l - 1],
                                      network.layers[l - 1].size);
    }

    free(dCost_dActv);

    return all_dCost_dZ;
}

void AddToSumNetwork(Network sumNetwork, Matrix all_dCost_dZ,
                     Matrix activationsLayers, Matrix inputs) {

    UpdateLayer(sumNetwork.layers[0], MatGetVector(all_dCost_dZ, 0), inputs);
    for (size_t i = 1; i < sumNetwork.depth; i++) {
        UpdateLayer(sumNetwork.layers[i], MatGetVector(all_dCost_dZ, i),
                    MatGetVector(activationsLayers, i - 1));
    }
    MatFree(all_dCost_dZ);
    MatFree(activationsLayers);
}

NNValue ValueToRemove(NNValue sumValue, NNValue inertiaValue,
                      TrainingSettings settings, size_t batch_size) {
    return -(1.0 - settings.inertia_strength) * sumValue / batch_size *
               settings.training_rate +
           settings.inertia_strength * inertiaValue;
}

void UpdateNetwork(Network network, Network sumNetwork, Network inertiaNetwork,
                   TrainingSettings settings, size_t batch_size) {
    for (size_t l = 0; l < network.depth; l++) {
        for (size_t i = 0; i < network.layers[l].size; i++) {

            Node node = network.layers[l].nodes[i];
            Node sumNode = sumNetwork.layers[l].nodes[i];
            Node *inertiaNodes = inertiaNetwork.layers[l].nodes;

            // update biases
            NNValue bdif = ValueToRemove(sumNode.bias, inertiaNodes[i].bias,
                                         settings, batch_size);
            node.bias += bdif;
            inertiaNodes[i].bias = -bdif;

            for (size_t j = 0; j < node.weight_size; j++) {

                // update weights
                NNValue wdif = ValueToRemove(sumNode.weights[j],
                                             inertiaNodes[i].weights[j],
                                             settings, batch_size);
                node.weights[j] += wdif;
                inertiaNodes[i].weights[j] = -wdif;
            }
        }
    }
}

NNValue BackPropagation(Network network, TrainingSettings settings,
                        InputBatch batch, Network inertiaNetwork) {
    NNValue accuracy = 0;

    Network sumNetwork = copyAndResetNetwork(network);

    for (size_t m = 0; m < batch.size; m++) {

        Matrix inputs = MatGetVector(batch.inputs, m);
        Matrix outputs = MatGetVector(batch.outputs, m);

        Matrix activationsLayers = PropagateAndKeep(inputs, network);

        size_t outputLayerI = network.depth - 1;
        accuracy += CostFunction(MatGetVector(activationsLayers, outputLayerI),
                                 outputs);

        Matrix all_dCost_dZ =
            BackPropagateInput(network, activationsLayers, outputs);

        AddToSumNetwork(sumNetwork, all_dCost_dZ, activationsLayers, inputs);
    }

    UpdateNetwork(network, sumNetwork, inertiaNetwork, settings, batch.size);

    freeNetwork(sumNetwork);

    return accuracy / batch.size;
}
