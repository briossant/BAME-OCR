#include "NeuralNetwork.h"
#include <stdio.h>

// sigmoid derivative with x = Sigmoid(...)
NNValue DSigmoid(NNValue x) { return x * (1 - x); }

// cost function derivative over the output activations
NNValue *FirstDCostDActv(NNValue *outputActivations, NNValue *expectedOutputs,
                         size_t outputSize) {
    NNValue *res = malloc(outputSize * sizeof(NNValue));
    for (size_t i = 0; i < outputSize; i++) {
        res[i] = 2 * (outputActivations[i] - expectedOutputs[i]);
    }
    return res;
}

void UpdateLayer(Layer layer, Matrix dCost, Matrix lastActv) {
    MatAdd(layer.biases, dCost);
    MatAdd(layer.weights, MatMult(dCost, lastActv));
}

NNValue *DCostDZ(NNValue *dCost_dActv, NNValue *activations, size_t size) {
    NNValue *dCost_dZ = malloc(sizeof(NNValue) * size);
    for (size_t i = 0; i < size; i++) {
        dCost_dZ[i] = dCost_dActv[i] * DSigmoid(activations[i]);
    }
    return dCost_dZ;
}

NNValue *GetNewDCostDActv(Layer layer, size_t size, NNValue *activations,
                          NNValue *dCost_dActv) {
    NNValue *new_dCost_dActv = malloc(size * sizeof(NNValue));
    for (size_t i = 0; i < size; i++) {

        NNValue dSig = DSigmoid(activations[i]);
        new_dCost_dActv[i] = 0;
        for (size_t j = 0; j < layer.size; j++) {
            new_dCost_dActv[i] +=
                layer.nodes[j].weights[i] * dSig * dCost_dActv[j];
        }
    }
    return new_dCost_dActv;
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

void AddToSumNetwork(Network sumNetwork, NNValue **all_dCost_dZ,
                     NNValue **activationsLayers, NNValue *inputs) {

    UpdateLayer(sumNetwork.layers[0], all_dCost_dZ[0], inputs);
    for (size_t i = 1; i < sumNetwork.depth; i++) {
        UpdateLayer(sumNetwork.layers[i], all_dCost_dZ[i],
                    activationsLayers[i - 1]);
    }

    for (size_t i = 0; i < sumNetwork.depth; i++) {
        free(all_dCost_dZ[i]);
        free(activationsLayers[i]);
    }
    free(all_dCost_dZ);
    free(activationsLayers);
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
        NNValue **activationsLayers =
            PropagateAndKeep(batch.inputs[m], network);

        size_t outputLayerI = network.depth - 1;
        accuracy +=
            CostFunction(activationsLayers[outputLayerI], batch.outputs[m],
                         network.layers[outputLayerI].size);

        NNValue **all_dCost_dZ =
            BackPropagateInput(network, activationsLayers, batch.outputs[m]);

        AddToSumNetwork(sumNetwork, all_dCost_dZ, activationsLayers,
                        batch.inputs[m]);
    }

    UpdateNetwork(network, sumNetwork, inertiaNetwork, settings, batch.size);

    freeNetwork(sumNetwork);

    return accuracy / batch.size;
}
