#include "../network/NeuralNetwork.h"
#include <stdlib.h>
#include "../utilities/printers.h"
#define INPUT_SIZE 28 * 28
#define OUTPUT_SIZE 10

void MnistTesting(Network network, char *dataset_path) {

    Matrix inputs;
    Matrix outputs;
    LoadMnist(&inputs, &outputs, dataset_path);

    NNValue final_value = TestPropagation(inputs, outputs, network);

    MatFree(inputs);
    MatFree(outputs);
    printf("//////////////////////////////////////////////////////////////////"
           "\n\n");
    printf("*** 🔴 Success rate: %f %% ***\n\n", final_value);
}

void MnistTraining(TrainingSettings settings, Network network,
                   char **datasets_path, size_t nbr_of_dataset) {
    printf("|||||||||||||||||| MNIST Training |||||||||||||||||||||\n\n");

    printNetwork(network);
    printf("\n");

    Matrix inputs;
    Matrix outputs;
    LoadMnist(&inputs, &outputs, datasets_path[0]);
    settings.nbr_of_inputs = inputs.w;
    for (size_t i = 1; i < nbr_of_dataset; ++i) {
        Matrix inputs_tmp;
        Matrix outputs_tmp;
        LoadMnist(&inputs_tmp, &outputs_tmp, datasets_path[i]);
        settings.nbr_of_inputs += inputs.w;
        inputs.mat =
            realloc(inputs.mat, settings.nbr_of_inputs * sizeof(NNValue *));
        outputs.mat =
            realloc(outputs.mat, settings.nbr_of_inputs * sizeof(NNValue *));
        for (size_t j = inputs.w; j < settings.nbr_of_inputs; ++j) {
            inputs.mat[j] = inputs_tmp.mat[j - inputs.w];
            outputs.mat[j] = outputs_tmp.mat[j - inputs.w];
        }
        inputs.w = settings.nbr_of_inputs;
        outputs.w = settings.nbr_of_inputs;
        free(inputs_tmp.mat);
        free(outputs_tmp.mat);
    }

    TrainNetwork(network, inputs, outputs, settings);
    MatFree(inputs);
    MatFree(outputs);
}
