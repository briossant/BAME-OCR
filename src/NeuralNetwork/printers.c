#include "NeuralNetwork.h"


void printNetwork(Network network){
    printf("///////////////////////////////\n");
    printf("network -> depth: %ld  input_size: %ld \n", network.depth, network.input_size);
    for (size_t i = 0; i < network.depth; i++) {
        printf("layer -> size: %ld \n", network.layers[i].size);
        for(size_t j=0; j< network.layers[i].size; j++) {
            Node nd = network.layers[i].nodes[j];
            printf("nd: %f %ld | ", nd.bias, nd.weight_size);
        }
        printf("\n");
    }
    printf("///////////////////////////////\n");
}

