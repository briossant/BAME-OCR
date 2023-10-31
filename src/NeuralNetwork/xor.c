#include "printers.h"
#include "NeuralNetwork.h"
#include <string.h>

#define INPUT_LAYER_SIZE 2
#define OUTPUT_LAYER_SIZE 1
#define CMD_COUNT 4

void PrintUsage(Network *network, size_t argc) {
    printf("Usage:\n");
    printf("〉help : show this message\n");
    printf("〉new [hidden layer size] ... : create a new network\n");
    printf("〉train [learning rate] [epochs] [inertia strength] : train the "
           "network\n");
    printf("〉run [0 or 1] [0 or 1] : run the prediction on those inputs\n");
    printf("\n");
}

size_t GetArgc(char *input) {
    char *p = input;
    size_t res = 0;
    while (*p != 0)
        if (*(p++) == ' ')
            ++res;
    return res;
}

void CreateNetwork(Network *network, size_t argc) {
    size_t *layers_size = malloc(sizeof(size_t) * (argc + 2));
    *layers_size = INPUT_LAYER_SIZE;
    *(layers_size + argc + 1) = OUTPUT_LAYER_SIZE;
    size_t i = 0;
    while (i++ < argc) {
        size_t x;
        if ((x = atoi(strtok(NULL, " "))) > 0)
            layers_size[i] = x;
        else {
            printf("Layer sizes must be positive integer.\n");
        }
    }
    *network = newNetwork(layers_size, argc + 2);
    printNetwork(*network);
}

void trainXOR(Network *network, size_t argc) {
    size_t trSetSize = 4;
    NNValue *inputsl[] = {(NNValue[]){0, 1}, (NNValue[]){1, 0},
                          (NNValue[]){0, 0}, (NNValue[]){1, 1}};
    NNValue *outputsl[] = {(NNValue[]){1}, (NNValue[]){1}, (NNValue[]){0},
                           (NNValue[]){0}};
    Matrix inputsm = {
        .w = trSetSize, .h = 2, .mat = outputsl, .label = "inputs"};
    Matrix outputsm = {
        .w = trSetSize, .h = 1, .mat = inputsl, .label = "outputs"};

    TrainingSettings sett = {.batch_size = 1, .nbr_of_inputs = trSetSize};
    sett.training_rate = strtod(strtok(NULL, " "), NULL);
    sett.epochs = atoi(strtok(NULL, " "));
    sett.inertia_strength = strtod(strtok(NULL, " "), NULL);

    TrainNetwork(*network, inputsm, outputsm, sett);
}

void testXOR(Network *network, size_t argc) {
    Matrix mat = MatInit(1, 2, 0, "input");
    mat.mat[0][0] = atoi(strtok(NULL, " "));
    mat.mat[0][1] = atoi(strtok(NULL, " "));

    Matrix res = Propagate(mat, *network);

    MatPrint(res);
}

struct parseEl {
    char *cmd_name;
    size_t min_argc;
    void (*fct)(Network *network, size_t argc);
};

struct parseEl parseList[] = {{"help", 0, PrintUsage},
                              {"new", 0, CreateNetwork},
                              {"train", 3, trainXOR},
                              {"run", 2, testXOR}};

int main(void) {
    PrintTitle();
    printf("Type \"help\" for more information.\n\n");
    Network *network = malloc(sizeof(Network));
    *network =
        newNetwork((size_t[]){INPUT_LAYER_SIZE, 42, OUTPUT_LAYER_SIZE}, 3);
    while (42) {
        // get user input
        char *input = NULL;
        size_t len = 0;
        printf("〉");
        if (getline(&input, &len, stdin) == 1)
            continue;

        // remove trailing newline from user inputs
        input[strcspn(input, "\n")] = 0;

        // get number of args and command name
        size_t argc = GetArgc(input);
        char *cmd_name = strtok(input, " ");

        // exec command or print error
        size_t i = 0;
        while (i < CMD_COUNT && strcmp(cmd_name, parseList[i].cmd_name) != 0)
            ++i;
        if (i == CMD_COUNT)
            printf("Unknown command '%s'. Type 'help' for help.\n", cmd_name);
        else if (argc < parseList[i].min_argc)
            printf("Not enough arguments. Type 'help' for help.\n");
        else
            parseList[i].fct(network, argc);

        free(input);
    }
    return 0;
}
