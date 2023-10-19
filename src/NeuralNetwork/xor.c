#include <stddef.h>
#include <stdio.h>
#include "NeuralNetwork.h"

void PrintTitle(){
    printf(" ▄▄▄▄    ▄▄▄       ███▄ ▄███▓▓█████     ▒█████   ▄████▄   ██▀███  \n");
    printf("▓█████▄ ▒████▄    ▓██▒▀█▀ ██▒▓█   ▀    ▒██▒  ██▒▒██▀ ▀█  ▓██ ▒ ██▒\n");
    printf("▒██▒ ▄██▒██  ▀█▄  ▓██    ▓██░▒███      ▒██░  ██▒▒▓█    ▄ ▓██ ░▄█ ▒\n");
    printf("▒██░█▀  ░██▄▄▄▄██ ▒██    ▒██ ▒▓█  ▄    ▒██   ██░▒▓▓▄ ▄██▒▒██▀▀█▄  \n");
    printf("░▓█  ▀█▓ ▓█   ▓██▒▒██▒   ░██▒░▒████▒   ░ ████▓▒░▒ ▓███▀ ░░██▓ ▒██▒\n");
    printf("░▒▓███▀▒ ▒▒   ▓▒█░░ ▒░   ░  ░░░ ▒░ ░   ░ ▒░▒░▒░ ░ ░▒ ▒  ░░ ▒▓ ░▒▓░\n");
    printf("▒░▒   ░   ▒   ▒▒ ░░  ░      ░ ░ ░  ░     ░ ▒ ▒░   ░  ▒     ░▒ ░ ▒░\n");
    printf(" ░    ░   ░   ▒   ░      ░      ░      ░ ░ ░ ▒  ░          ░░   ░ \n");
    printf(" ░            ░  ░       ░      ░  ░       ░ ░  ░ ░         ░     \n");
    printf("      ░                                         ░                \n\n\n");
}


void train(size_t epoch, NNValue trRate) {
    printf("~|~ Training ~|~\n\n");

    size_t layers_size[] = {2, 42, 42, 42, 1};
    Network network = newNetwork(layers_size, 5);

    size_t trSetSize = 4;

    NNValue *inputsl[] = {(NNValue[]){0, 1}, (NNValue[]){1, 0},
                          (NNValue[]){0, 0}, (NNValue[]){1, 1}};

    NNValue *outputsl[] = {(NNValue[]){1}, (NNValue[]){1}, (NNValue[]){0},
                           (NNValue[]){0}};

    Matrix inputsm = {.w = trSetSize, .h = 2, .mat = outputsl, .label = "inputs"};
    Matrix outputsm = {.w = trSetSize, .h = 1, .mat = inputsl, .label = "outputs"};

    TrainingSettings sett = {.training_rate = trRate,
                             .batch_size = 1,
                             .epochs = epoch,
                             .inertia_strength = 0,
                             .nbr_of_inputs = trSetSize};

    TrainNetwork(network, inputsm, outputsm, sett);

    // create new fct to propagate on multi inputs layer matrix
    Matrix res = Propagate(inputsm, network);
    printf("Final tests:  \n");
    MatPrint(res);
}

void save(char *filepath) {
    printf("saving\n");
    size_t layers_size[] = {2, 4, 2};
    Network network = newNetwork(layers_size, 3);
    // SaveNetwork( network,filepath);
}

void load(char *filepath) {
    printf("loading\n");
    // Network network = LoadNetwork(filepath);
}

void solve(char input[2]) {
    size_t layers_size[] = {2, 4, 2};
    Network network = newNetwork(layers_size, 3);

    printNetwork(network);

    NNValue *list = malloc(sizeof(NNValue) * 2);
    list[0] = (NNValue)input[0];
    list[1] = (NNValue)input[1];
    Matrix mat = {.h = 2, .w = 1, .mat = &list};
    Matrix res = Propagate(mat, network);

    printf("Result of %d%d :\n", input[0], input[1]);
    MatPrint(res);
}

void printHelp() {
    printf("Usage:\n./xor t [Epoch] [TrainingRate]: for training\n./xor p "
           "[0-1] [0-1] : propagate inputs");
    printf("\n./xor s [filepath] : save\n./xor l [filepath] : to load\n");
    printf("./xor m [Epochs] [TrainingRate] [BatchSize] [InertiaStrength (0.0 "
           "to 1.0)] : train on mnist\n");
}

void mnist(size_t epoch, NNValue learning_rate, size_t batch_size,
           NNValue inertia_strength) {
    TrainingSettings sett = {
        .training_rate = learning_rate,
        .batch_size = batch_size,
        .epochs = epoch,
        .inertia_strength = inertia_strength,
        .nbr_of_inputs = 42 // dummy value
    };
    MnistTraining(sett);
}

int main(int argc, char **argv) {
    PrintTitle();
    if (argc < 2) {
        printHelp();
        return 1;
    }
    srand(time(NULL));

    if (argv[1][0] == 'p') {
        if (argc < 4) {
            printHelp();
            return 1;
        }

        char input[] = {argv[2][0] - 48, argv[3][0] - 48};

        solve(input);
    } else if (argv[1][0] == 'm') {
        if (argc < 6) {
            printHelp();
            return 1;
        }
        size_t epoch;
        size_t batch_size;
        sscanf(argv[2], "%zu", &epoch);
        sscanf(argv[4], "%zu", &batch_size);
        mnist(epoch, (NNValue)atof(argv[3]), batch_size,
              (NNValue)atof(argv[5]));
    } else if (argv[1][0] == 't') {
        if (argc < 4) {
            printHelp();
            return 1;
        }
        size_t epoch;
        sscanf(argv[2], "%zu", &epoch);
        train(epoch, (NNValue)atof(argv[3]));
    } else if (argv[1][0] == 's') {
        if (argc < 3) {
            printHelp();
            return 1;
        }
        save(argv[2]);
    } else if (argv[1][0] == 'l') {
        if (argc < 3) {
            printHelp();
            return 1;
        }
        load(argv[2]);
    } else
        printHelp();

    return 0;
}
