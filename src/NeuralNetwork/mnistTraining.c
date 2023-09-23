#include "NeuralNetwork.h"
#include <stdlib.h>

#define INPUT_SIZE 28*28
#define OUTPUT_SIZE 10

#define NBR_OF_TRAINING_INPUTS 60000
#define NBR_OF_TESTING_INPUTS 10000

void MnistTraining(TrainingSettings settings) {
    settings.nbr_of_inputs = NBR_OF_TRAINING_INPUTS;
    
    size_t number_of_layers = 4;
    size_t layers_size[] = {INPUT_SIZE, 16, 16, OUTPUT_SIZE};
    Network network = newNetwork(layers_size, number_of_layers);

    double **inputs;
    double **outputs;
    LoadMnist(&inputs, &outputs, True);

    TrainNetwork(network, inputs, outputs, settings);
   
    // todo: test the network with testing data
    //LoadMnist(&inputs, &outputs, False);
}

