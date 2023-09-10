#include "NeuralNetwork.h"


double startingValue() {
    return ((double)rand() / RAND_MAX * 2.0 - 1.0);
}

Node newNode(size_t weight_size) {
    double *weights = malloc(weight_size * sizeof(double));
    for (size_t i = 0;i < weight_size;i++) {
        weights[i] = startingValue();
    }
    Node nd = {
        .weight_size = weight_size,
        .bias = startingValue(),
        .weights = weights
    };
    return nd;
}


Layer newLayer(size_t layer_size, size_t before_layer_size) {
    if (layer_size < 1) {
        errx(0, "a layer needs at least one node (layers_size < 1)");
    }

    Node *nodes = malloc(sizeof(Node) * layer_size);

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

    // -1 because the input layer isn't a proper layer
    Layer *layers = malloc((number_of_layers-1) * sizeof(Layer)); 

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



