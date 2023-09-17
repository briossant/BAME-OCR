#include <stdio.h>
#include "NeuralNetwork.h"

void shuffle(double *input[], double* output[], size_t n)
{
    if (n > 1) 
    {
        for (size_t i = 0; i < n - 1; i++) 
        {
            size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
            double* t = input[j];
            input[j] = input[i];
            input[i] = t;
            t = output[j];
            output[j] = output[i];
            output[i] = t;
        }
    }
}


void train(size_t epoch, double trRate) {
    printf("~|~ Training ~|~\n\n");
    
    size_t layers_size[] = {2, 16, 16, 1};
    Network network = newNetwork(layers_size, 3);
    
    size_t trSetSize = 4;
    double *inputs[] =  {(double[]) {0,1}, (double[]){1,0}, 
        (double[]){0,0}, (double[]){1,1}};
    double *outputs[] = {(double[]){1},(double[]){1},
        (double[]){0},(double[]){0}};
    
    for (size_t i = 0; i < epoch; i++) {
        shuffle(inputs, outputs, trSetSize); 
        double err = BackPropagation(network, trRate, inputs, outputs, trSetSize);
        printNetwork(network);
        printf("Training Epoch: %ld -> Accuracy: %f\n\n",i, err);
    }


}



void save(char *filepath) {
    printf("saving\n");
    size_t layers_size[] = {2, 4, 2};
    Network network = newNetwork(layers_size, 3);
    //SaveNetwork( network,filepath);

}

void load(char *filepath){
    printf("loading\n");
}

void solve(char input[2]) {
    size_t layers_size[] = {2, 4, 2};
    Network network = newNetwork(layers_size, 3);
    
    printNetwork(network);

    double inputs[2] = {(double)input[0], (double)input[1]};
    double *res = Propagate(inputs, network);

    printf("Result of %d%d -> 0:%f 1:%f\n", input[0], input[1], res[0], res[1]);
}


void printHelp() {
    printf("Usage:\n./xor t [Epoch] [TrainingRate]: for training\n./xor p [0-1] [0-1] : propagate inputs");
    printf("\n./xor s [filepath] : save\n./xor l [filepath] : to load\n"); 
}

int main(int argc, char **argv) {
    if (argc < 2){
        printHelp();
        return 1;
    }
    srand(time(NULL));


    if(argv[1][0] == 'p') {
        if (argc < 4) {
            printHelp();
            return 1;
        }

        char input[] = {argv[2][0]-48, argv[3][0]-48};

        solve(input);
    } else if(argv[1][0] == 't') { 
        if (argc < 4) {
            printHelp();
            return 1;
        }
        size_t epoch;
        sscanf(argv[2], "%zu", &epoch);
        train(epoch, atof(argv[3]));
    } else if(argv[1][0] == 's') {
        if (argc < 3) {
            printHelp();
            return 1;
        }
        save(argv[2]);
    } else if(argv[1][0] == 'l') {
        if (argc < 3) {
            printHelp();
            return 1;
        }
        load(argv[2]);
    }else printHelp();

    return 0;
}

