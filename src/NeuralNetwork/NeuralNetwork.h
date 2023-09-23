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

typedef double NNValue;

typedef struct InputBatch {
    size_t size; 
    NNValue **inputs;
    NNValue **outputs;
} InputBatch;

typedef struct TrainingSettings {
    size_t nbr_of_inputs;
    NNValue training_rate;
    size_t epochs;
    size_t batch_size;
} TrainingSettings;


typedef struct Node {
    size_t weight_size; 
    NNValue bias;
    NNValue *weights;  
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

typedef enum { False = 0, True = 1 } Bool;


////////////////////////////////////
// NetworkInit

Network newNetwork(size_t *layers_size, size_t number_of_layers);

Layer newLayer(size_t layer_size, size_t before_layer_size);

Node newNode(size_t weight_size);

NNValue startingValue(); // don't forget to init rand() with srand(time(NULL));

Network copyAndResetNetwork(Network network);

void freeNetwork(Network network);



////////////////////////////////////
// Printers

void printNetwork(Network network);



////////////////////////////////////
// Mnist Loading

void LoadMnist(NNValue*** images, NNValue*** labels, Bool isForTraining);

void MnistTraining(TrainingSettings settings);

////////////////////////////////////
// Propagation

NNValue* Propagate(NNValue *inputs, Network network);

NNValue** PropagateAndKeep(NNValue* inputs, Network network);

NNValue *PropagateLayer(NNValue *lastActivation, Layer layer);

NNValue WeightedSum(NNValue *activations, Node node);

// may switch to ReLU function later on
NNValue Sigmoid(NNValue x);



////////////////////////////////////
// Back Propagation

NNValue BackPropagation(Network network, NNValue training_rate, InputBatch batch);



////////////////////////////////////
// Training

void TrainNetwork(Network network, NNValue **inputs, 
        NNValue** outputs, TrainingSettings settings);



////////////////////////////////////
// Saving and loading

void SaveNetwork(Network network, char *filepath);

Network LoadNetwork(char *filepath);



#endif
