#include "NeuralNetwork.h"

extern ActivationFunctions SigmoidFcts;
extern ActivationFunctions ReLUFcts;

NNValue startingValue() {
    return ((NNValue)rand() / RAND_MAX * 2.0 - 1.0); // * STARTING_RANGE -> may
                                                     // add it later
}

Layer newLayer(size_t layer_size, size_t before_layer_size) {
    if (layer_size < 1) {
        errx(0, "a layer needs at least one node (layers_size < 1)");
    }
    Layer layer = {.size = layer_size,
                   .activFcts = SigmoidFcts,
                   .biases =
                       MatInitWithFct(1, layer_size, startingValue, "Biases"),
                   .weights = MatInitWithFct(before_layer_size, layer_size,
                                             startingValue, "Weights")};

    return layer;
}

// Layer new_layer(Node *node, size_t layer_size) {
//    Layer layer = {.size = layer_size, .nodes = node};
//   return layer;
//}

Network newNetwork(size_t *layers_size, size_t number_of_layers) {
    if (number_of_layers < 2) {
        errx(0, "a neural network has a least 2 layers : input and output. "
                "(number_of_layers < 2)");
    }

    // -1 because the input layer isn't a proper layer
    Layer *layers = malloc((number_of_layers - 1) * sizeof(Layer));

    for (size_t i = 0; i < number_of_layers - 1; ++i) {
        layers[i] = newLayer(layers_size[i + 1], layers_size[i]);
    }

    Network network = {.input_size = layers_size[0],
                       .depth = number_of_layers - 1,
                       .layers = layers};

    return network;
}

Network copyAndResetNetwork(Network network) {
    Layer *layers = malloc(sizeof(Layer) * network.depth);
    Network newNetwork = {.input_size = network.input_size,
                          .depth = network.depth,
                          .layers = layers};

    size_t weights_size = network.input_size;

    for (size_t l = 0; l < network.depth; l++) {
        size_t layer_size = network.layers[l].size;

        Layer layer = {.size = layer_size,
                       .biases = MatInit(1, layer_size, 0.0, "Biases"),
                       .weights =
                           MatInit(weights_size, layer_size, 0.0, "Weights")};
        weights_size = layer_size;
        layers[l] = layer;
    }

    return newNetwork;
}

void freeNetwork(Network network) {
    for (size_t l = 0; l < network.depth; l++) {
        MatFree(network.layers[l].weights);
        MatFree(network.layers[l].biases);
    }
    free(network.layers);
}
