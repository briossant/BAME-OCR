#include "../network/NeuralNetwork.h"
#include <stdlib.h>
#include "../utilities/printers.h"
#define INPUT_SIZE 28 * 28
#define OUTPUT_SIZE 10

void MnistTesting(Network network) {

    Matrix inputs;
    Matrix outputs;
    LoadMnist(&inputs, &outputs, False);

    NNValue final_value = TestPropagation(inputs, outputs, network);

    MatFree(inputs);
    MatFree(outputs);
    printf("//////////////////////////////////////////////////////////////////"
           "\n\n");
    printf("*** 🔴 Success rate: %f %% ***\n\n", final_value);
}

void MnistTraining(TrainingSettings settings, Network network) {
    printf("|||||||||||||||||| MNIST Training |||||||||||||||||||||\n\n");

    printNetwork(network);
    printf("\n");

    Matrix inputs;
    Matrix outputs;
    LoadMnist(&inputs, &outputs, True);
    settings.nbr_of_inputs = inputs.w;

    TrainNetwork(network, inputs, outputs, settings);
    MatFree(inputs);
    MatFree(outputs);
}
