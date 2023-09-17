#include <stdio.h>
#include "NeuralNetwork.h"


void train() {
    printf("~|~ Training ~|~\n\n");
    
    size_t layers_size[] = {2, 4, 1};
    Network network = newNetwork(layers_size, 3);
    
    double *inputs[] =  {(double[]) {0,1}, (double[]){1,0}, 
        (double[]){0,0}, (double[]){1,1}};
    double *outputs[] = {(double[]){1},(double[]){1},
        (double[]){0},(double[]){0}};
    
    for (size_t i = 0; i < 10; i++) {
        double err = BackPropagation(network, inputs, outputs, 4);
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
    printf("Usage:\n./xor t : for training\n./xor p [0-1] [0-1] : propagate inputs");
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
        train();
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

