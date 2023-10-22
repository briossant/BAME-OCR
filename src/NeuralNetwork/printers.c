#include "NeuralNetwork.h"

void printNetwork(Network network) {
    printf("〉⎧⏵⏵⏵ NETWORK 🕸\n");
    printf("〉⎬> Settings: depth: %ld | input_size: %ld \n〉⎬> Layers:\n",
           network.depth, network.input_size);
    for (size_t i = 0; i < network.depth; i++) {
        printf("〉⎬⎼[%ld]> size: %ld \n", i, network.layers[i].size);
    }
    printf("〉⎩⏴⏴⏴ NETWORK END\n\n");
}
