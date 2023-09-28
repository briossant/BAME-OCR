#ifndef ACTIVATION_FUNCTIONS
#define ACTIVATION_FUNCTIONS

#include "NeuralNetwork.h"

/////////////////////////////////////
/// ZFunctions

NNValue WeightedSum(NNValue *activations, Node node);



/////////////////////////////////////
/// SquishingFunctions

// may switch to ReLU function later on
NNValue Sigmoid(NNValue x);

#endif 
