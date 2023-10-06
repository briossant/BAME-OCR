#include "NeuralNetwork.h"

NNValue startingValue() {
    return ((NNValue)rand() / RAND_MAX * 2.0 - 1.0); // * STARTING_RANGE -> may
                                                     // add it later
}

Node newNode(size_t weight_size) {
    NNValue *weights = malloc(weight_size * sizeof(NNValue));
    for (size_t i = 0; i < weight_size; i++) {
        weights[i] = startingValue();
    }
    Node nd = {.weight_size = weight_size,
               .bias = startingValue(),
               .weights = weights};
    return nd;
}

Node new_node(size_t weight_size, double *weights, double bias) {

    Node nd = {.weight_size = weight_size, .bias = bias, .weights = weights};
    return nd;
}

Layer newLayer(size_t layer_size, size_t before_layer_size) {
    if (layer_size < 1) {
        errx(0, "a layer needs at least one node (layers_size < 1)");
    }

    Node *nodes = malloc(sizeof(Node) * layer_size);

    for (size_t i = 0; i < layer_size; ++i) {
        nodes[i] = newNode(before_layer_size);
    }

    Layer layer = {.size = layer_size, .nodes = nodes};

    return layer;
}

Layer new_layer(Node *node, size_t layer_size) {
    Layer layer = {.size = layer_size, .nodes = node};
    return layer;
}
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
        Node *nodes = malloc(sizeof(Node) * layer_size);

        for (size_t i = 0; i < layer_size; ++i) {
            NNValue *weights = malloc(weights_size * sizeof(NNValue));
            for (size_t i = 0; i < weights_size; i++) {
                weights[i] = 0.0;
            }
            Node nd = {
                .weight_size = weights_size, .bias = 0.0, .weights = weights};
            nodes[i] = nd;
        }
        weights_size = layer_size;
        Layer layer = {.size = layer_size, .nodes = nodes};
        layers[l] = layer;
    }

    return newNetwork;
}

void freeNetwork(Network network) {
    for (size_t l = 0; l < network.depth; l++) {
        for (size_t i = 0; i < network.layers[l].size; ++i) {
            free(network.layers[l].nodes[i].weights);
        }
        free(network.layers[l].nodes);
    }
    free(network.layers);
}
