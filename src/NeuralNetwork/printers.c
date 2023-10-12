#include "NeuralNetwork.h"

void printNetwork(Network network) {
    printf("\n~~[{- ~=~  THE NETWORK  ~=~ -}]~~\n\n");
    printf("~> Settings: depth: %ld | input_size: %ld \n\n~> Layers:\n",
           network.depth, network.input_size);
    for (size_t i = 0; i < network.depth; i++) {
        printf("[%ld]> size: %ld \n", i, network.layers[i].size);
    }
    printf("\n~~[{- ~=~  NETWORK END  ~=~ -}]~~\n\n");
}
