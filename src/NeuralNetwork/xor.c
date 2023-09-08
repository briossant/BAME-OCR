#include <stdio.h>
#include "NeuralNetwork.h"

void train();


void solve(char input[2]) {
    size_t layers_size[] = {2, 1};
    Network test = newNetwork(layers_size, 2);
    
    printf("todo... %d %d  %ld\n", input[0], input[1], test.depth);
}


void printHelp() {
    printf("Usage:\n./xor t : for training\n./xor s [0-1] [0-1]: solve inputs\n"); 
}

int main(int argc, char **argv) {
    if (argc < 2){
        printHelp();
        return 1;
    }

    if(argv[1][0] == 's') {
        if (argc < 4) {
            printHelp();
            return 1;
        }

        char input[] = {argv[2][0]-48, argv[3][0]-48};

        solve(input);
    }

    return 0;
}

