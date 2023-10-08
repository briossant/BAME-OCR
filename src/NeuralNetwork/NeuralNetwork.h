#ifndef NEURAL_NETWORK
#define NEURAL_NETWORK

#include <err.h>
#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "matrices/matrices.h"
#include "precision.h"

//////////////////////////////////////
// Struct

typedef struct InputBatch {
    size_t size;
    Matrix inputs;
    Matrix outputs;
} InputBatch;

typedef struct TrainingSettings {
    size_t nbr_of_inputs;
    NNValue training_rate;
    NNValue inertia_strength; // between 0.0 and 1.0
    size_t epochs;
    size_t batch_size;
} TrainingSettings;

typedef struct Layer {
    size_t size; // number of nodes
    Matrix biases;
    Matrix weights;
} Layer;

typedef struct Network {
    size_t input_size; // number of input nodes
    size_t depth; // number of layers (hidden layers+output layer) i.e Size of
                  // layers[]
    Layer *layers;
} Network;

////////////////////////////////////
// NetworkInit

Network newNetwork(size_t *layers_size, size_t number_of_layers);

Layer newLayer(size_t layer_size, size_t before_layer_size);

// Layer new_layer (Node* node,  size_t layer_size);

// Node new_node( size_t weight_size, double* weights , double bias);

NNValue startingValue(); // don't forget to init rand() with srand(time(NULL));

Network copyAndResetNetwork(Network network);

void freeNetwork(Network network);

////////////////////////////////////
// Printers

void printNetwork(Network network);

////////////////////////////////////
// Mnist Loading

void LoadMnist(Matrix *images, Matrix *labels, Bool isForTraining);

void MnistTraining(TrainingSettings settings);

////////////////////////////////////
// Propagation

Matrix Propagate(Matrix inputs, Network network);

Matrix *PropagateAndKeep(Matrix inputs, Network network);

Matrix PropagateLayer(Matrix lastActivation, Layer layer);

Matrix WeightedSum(Matrix activations, Layer layer);

// may switch to ReLU function later on
NNValue Sigmoid(NNValue x);

NNValue TestPropagation(Matrix inputs, Matrix outputs, Network network);

NNValue CostFunction(Matrix outputActivations, Matrix expectedOutputs);

////////////////////////////////////
// Back Propagation

NNValue BackPropagation(Network network, TrainingSettings settings,
                        InputBatch batch, Network inertiaNetwork);

////////////////////////////////////
// Training

void TrainNetwork(Network network, Matrix inputs, Matrix outputs,
                  TrainingSettings settings);

////////////////////////////////////
// Saving and loading

void SaveNetwork(Network network, char *filepath);

Network LoadNetwork(char *filepath);

#endif
