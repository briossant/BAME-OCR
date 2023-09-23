#include "NeuralNetwork.h"
#include <stdlib.h>

#define INPUT_SIZE 28*28
#define OUTPUT_SIZE 10

#define NBR_OF_TRAINING_INPUTS 60000
#define NBR_OF_TESTING_INPUTS 10000

void MnistTraining(TrainingSettings settings) {
    printf("|||||||||||||||||| MNIST Training |||||||||||||||||||||\n\n");

    settings.nbr_of_inputs = NBR_OF_TRAINING_INPUTS;
    
    size_t number_of_layers = 4;
    size_t layers_size[] = {INPUT_SIZE, 42, 16, OUTPUT_SIZE};
    Network network = newNetwork(layers_size, number_of_layers);
    
    printNetwork(network);
    printf("\n");

    NNValue **inputs;
    NNValue **outputs;
    LoadMnist(&inputs, &outputs, True);

    TrainNetwork(network, inputs, outputs, settings);
   
    // todo: test the network with testing data
    //LoadMnist(&inputs, &outputs, False);
}

