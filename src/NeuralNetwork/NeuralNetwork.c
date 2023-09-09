#include "NeuralNetwork.h"
#include <err.h>
#include <stddef.h>



Node newNode(size_t weight_size) {
    double weights[weight_size];
    for (size_t i = 0;i < weight_size;i++) {
        weights[i] = 0.5; // replace later with a random value 
    }
    Node nd = {
        .weight_size = weight_size,
        .bias = 0.0, // replace later with a random value
        .weights = weights
    };
    return nd;
}

Layer newLayer(size_t layer_size, size_t before_layer_size) {
    if (layer_size < 1) {
        errx(0, "a layer needs at least one node (layers_size < 1)");
    }

    Node nodes[layer_size];

    for (size_t i = 0; i < layer_size;++i) {
        nodes[i] = newNode(before_layer_size);
    }

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
        layers[i] = newLayer(layers_size[i+1], layers_size[i]);    
    }

    Network network = {
        .input_size = layers_size[0],
        .depth = number_of_layers-1,
        .layers = layers
    };

    return network;
}


