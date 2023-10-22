#include "NeuralNetwork.h"
#include <stdlib.h>

#define INPUT_SIZE 28 * 28
#define OUTPUT_SIZE 10

void MnistTraining(TrainingSettings settings) {
    printf("|||||||||||||||||| MNIST Training |||||||||||||||||||||\n\n");

    size_t number_of_layers = 3;
    size_t layers_size[] = {INPUT_SIZE, 16, OUTPUT_SIZE};
    Network network = newNetwork(layers_size, number_of_layers);

    printNetwork(network);
    printf("\n");

    Matrix inputs;
    Matrix outputs;
    LoadMnist(&inputs, &outputs, True);
    settings.nbr_of_inputs = inputs.w;

    TrainNetwork(network, inputs, outputs, settings);

    LoadMnist(&inputs, &outputs, False);

    NNValue final_value = TestPropagation(inputs, outputs, network);

    printf("//////////////////////////////////////////////////////////////////"
           "\n\n");
    printf("*** Final accuracy: %f %% ***\n\n", final_value);
}
