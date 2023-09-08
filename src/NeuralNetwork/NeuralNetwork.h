#ifndef NEURAL_NETWORK
#define NEURAL_NETWORK

#include <stddef.h>

typedef struct Node {
    size_t weight_size; 
    double value;
    double *weights;  
} Node;


typedef struct Layer {
    size_t size; // number of nodes
    Node *nodes;
} Layer;


typedef struct Network {
    size_t input_size; // number of input nodes
    size_t depth; // number of layers (hidden layers+output layer) i.e Size of layers[]
    Layer *layers;
} Network;


Network newNetwork(size_t *layers_size, size_t number_of_layers);

Layer newLayer(size_t layer_size);

Node newNode();


#endif
