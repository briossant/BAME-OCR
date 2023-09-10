#ifndef NEURAL_NETWORK
#define NEURAL_NETWORK

#include <stddef.h>

//////////////////////////////////////
// Struct

typedef struct Node {
    size_t weight_size; 
    double bias;
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



////////////////////////////////////
// NetworkInit

Network newNetwork(size_t *layers_size, size_t number_of_layers);

Layer newLayer(size_t layer_size, size_t before_layer_size);

Node newNode(size_t weight_size);



////////////////////////////////////
// Propagation

double* Propagate(double *inputs, Network network);

double *PropagateLayer(double *lastActivation, Layer layer);

double WeightedSum(double *activations, Node node);

double Sigmoid(double x);

#endif
