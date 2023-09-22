#ifndef NEURAL_NETWORK
#define NEURAL_NETWORK

#include <stddef.h>
#include <err.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


//////////////////////////////////////
// Struct

typedef struct InputBatch {
    size_t size; 
    double **inputs;
    double **outputs;
} InputBatch;

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

double startingValue(); // don't forget to init rand() with srand(time(NULL));

Network copyAndResetNetwork(Network network);

void freeNetwork(Network network);



////////////////////////////////////
// Printers

void printNetwork(Network network);



////////////////////////////////////
// Mnist Loading

double **loadMnistImages(); 



////////////////////////////////////
// Propagation

double* Propagate(double *inputs, Network network);

double** PropagateAndKeep(double* inputs, Network network);

double *PropagateLayer(double *lastActivation, Layer layer);

double WeightedSum(double *activations, Node node);

// may switch to ReLU function later on
double Sigmoid(double x);



////////////////////////////////////
// Back Propagation

double BackPropagation(Network network, double training_rate, InputBatch batch);



////////////////////////////////////
// Training

void trainNetwork(Network network, double **inputs, 
        double** outputs, size_t n, double trainingRate, 
        size_t epochs);



////////////////////////////////////
// Saving and loading

void SaveNetwork(Network network, char *filepath);

Network LoadNetwork(char *filepath);



#endif
