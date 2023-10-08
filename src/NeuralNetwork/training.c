#include "NeuralNetwork.h"
#include <stddef.h>
#include <stdlib.h>

void shuffle(NNValue **input, NNValue **output, size_t n) {
    for (size_t i = 0; i < n - 1; i++) {
        size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
        NNValue *t = input[j];
        input[j] = input[i];
        input[i] = t;
        t = output[j];
        output[j] = output[i];
        output[i] = t;
    }
}

// Separate input and output matrices into sub-matrices
InputBatch *MakeInputsBatch(Matrix inputs, Matrix outputs, size_t nbr_of_batch,
                            size_t batch_size) {
    InputBatch *res = malloc(sizeof(InputBatch) * nbr_of_batch);
    for (size_t i = 0; i < nbr_of_batch; i++) {
        Matrix input = {
            .w = batch_size, .h = inputs.h, .mat = inputs.mat + i * batch_size};
        Matrix output = {.w = batch_size,
                         .h = outputs.h,
                         .mat = outputs.mat + i * batch_size};

        InputBatch batch = {
            .size = batch_size, .inputs = input, .outputs = output};
        res[i] = batch;
    }
    return res;
}

void TrainNetwork(Network network, Matrix inputs, Matrix outputs,
                  TrainingSettings settings) {
    printf("~~[- Training -]~~\n");
    size_t nbr_of_batch = settings.nbr_of_inputs / settings.batch_size;
    printf("nbr_of_batch: %ld | nbr_of_inputs: %ld | batch_size: %ld | "
           "training_rate: %f | epochs: %ld | inertia_strength: %f\n",
           nbr_of_batch, settings.nbr_of_inputs, settings.batch_size,
           settings.training_rate, settings.epochs, settings.inertia_strength);

    InputBatch *batchs =
        MakeInputsBatch(inputs, outputs, nbr_of_batch, settings.batch_size);

    for (size_t i = 0; i < settings.epochs; i++) {
        shuffle(inputs.mat, outputs.mat, settings.nbr_of_inputs);
        NNValue err = 0.0;
        Network inertiaNetwork = copyAndResetNetwork(network);

        for (size_t j = 0; j < nbr_of_batch; j++) {
            err +=
                BackPropagation(network, settings, batchs[j], inertiaNetwork);
        }

        err /= nbr_of_batch;
        // if (i % (1 + epochs / 100) == 0);
        printf("~~{ [Training Epoch]: %ld -> [Accuracy]: %f }\n", i, err);
    }
}
