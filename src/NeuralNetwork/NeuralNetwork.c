#include "NeuralNetwork.h"
#include <err.h>
#include <stddef.h>



Layer newLayer(size_t layer_size) {
    if (layer_size < 1) {
        errx(0, "a layer needs at least one node (layers_size < 1)");
    }

    Node *nodes;

    for (size_t i = 0; i < layer_size;++i) {}

    Layer layer = {
        .size = layer_size,
        .nodes = nodes
    };

    return layer;
}





Network newNetwork(size_t *layers_size, size_t number_of_layers) {
    if (number_of_layers < 2) {
        errx(0, "a neural network has a least 2 layers : input and output. (number_of_layers < 2)");
    }

    Layer layers[number_of_layers-1]; // -1 because the input layer isn't a proper layer

    for (size_t i = 0; i < number_of_layers-1;++i) {
        layers[i] = newLayer(layers_size[i+1]);    
    }

    Network network = {
        .input_size = layers_size[0],
        .depth = number_of_layers-1,
        .layers = layers
    };

    return network;
}


