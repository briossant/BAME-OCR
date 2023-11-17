#include "NeuralNetwork.h"

// sigmoid derivative with x = Sigmoid(...)
NNValue DSigmoid(NNValue x) { return x * (1 - x); }

NNValue Sigmoid(NNValue x) { return 1 / (1 + exp(-x)); }

ActivationFunctions SigmoidFcts = {.dSquishFct = DSigmoid,
                                   .squishFct = Sigmoid};

////////////////////////////////////////

NNValue ReLU(NNValue x) { return x > 0 ? x : 0; }

NNValue DReLU(NNValue x) { return x > 0 ? 1 : 0; }

ActivationFunctions ReLUFcts = {.dSquishFct = DReLU, .squishFct = ReLU};
