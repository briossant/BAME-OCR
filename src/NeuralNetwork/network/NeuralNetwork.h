#ifndef NEURAL_NETWORK
#define NEURAL_NETWORK

#include <err.h>
#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../matrices/matrices.h"
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

// Create a new Network
// size_t *layers_size : list containing the size of each layers (from input to
// output layer)
// size_t number_of_layers : size of layers_size list
Network newNetwork(size_t *layers_size, size_t number_of_layers);

Layer newLayer(size_t layer_size, size_t before_layer_size);

// Layer new_layer (Node* node,  size_t layer_size);

// Node new_node( size_t weight_size, double* weights , double bias);

// function returning the init value of a weight or bias
// don't forget to init rand() with srand(time(NULL));
NNValue startingValue();

// create a new network with the same size of Network network
// and put each weights and biases to 0.0
Network copyAndResetNetwork(Network network);

void freeNetwork(Network network);

////////////////////////////////////
// Mnist Loading

// load mnist data into Matrix *images and Matrix *labels
// Bool isForTraining select between the 2 2 differents path define in
// loadMnist.c
void LoadMnist(Matrix *images, Matrix *labels, char *path);
// don't forget to init rand() with srand(time(NULL));
void MnistTraining(TrainingSettings settings, Network network,
                   char *dataset_path);

void MnistTesting(Network network, char *dataset_path);

////////////////////////////////////
// Propagation

Matrix Propagate(Matrix inputs, Network network);

Matrix *PropagateAndKeep(Matrix inputs, Network network);

Matrix PropagateLayer(Matrix lastActivation, Layer layer);

Matrix WeightedSum(Matrix activations, Layer layer);

// may switch to ReLU function later on
NNValue Sigmoid(NNValue x);

// propagate on all of the input layer of the Matrix inputs and compare it with
// the Matrix outputs to return a success rate (in %)
NNValue TestPropagation(Matrix inputs, Matrix outputs, Network network);

NNValue CostFunction(Matrix outputActivations, Matrix expectedOutputs);

int Predict(Network network, Matrix inputs);

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
