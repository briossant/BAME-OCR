#include "network/NeuralNetwork.h"
#include "../ImageProcessing/ImageProcess.h"
#include "utilities/printers.h"
#include <string.h>

#define INPUT_LAYER_SIZE 784
#define OUTPUT_LAYER_SIZE 10
#define CMD_COUNT 7

void PrintUsage(Network *network, size_t argc) {
    (void)network;
    (void)argc;

    printf("\n🟢 Usage:\n");
    printf("〉help : show this message\n");
    printf("〉quit : exit program\n");
    printf("〉new [hidden layer size] ... : create a new network\n");
    printf("〉train [learning rate] [epochs] [inertia strength] [batch size] "
           "[dataset] : "
           "train the network\n");
    printf(
        "〉test [dataset] : test network performance on the test database\n");
    printf("〉save [filename] : save the network\n");
    printf("〉load [filename] : load the network\n");
    printf(
        "〉img [filename] : predict the number on the image (28x28 pixels)\n");
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

void callSave(Network *network, size_t argc) {
    (void)argc;

    SaveNetwork(*network, strtok(NULL, " "));
}

void callLoad(Network *network, size_t argc) {
    (void)argc;

    freeNetwork(*network);
    *network = LoadNetwork(strtok(NULL, " "));
    printNetwork(*network);
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
    freeNetwork(*network);
    *network = newNetwork(layers_size, argc + 2);
    printNetwork(*network);
    free(layers_size);
}

void callTraining(Network *network, size_t argc) {

    TrainingSettings sett = {.nbr_of_inputs = 42}; // 42 is dummy value (will be
                                                   // change by MnistTraining)
    sett.training_rate = strtod(strtok(NULL, " "), NULL);
    sett.epochs = atoi(strtok(NULL, " "));
    sett.inertia_strength = strtod(strtok(NULL, " "), NULL);
    sett.batch_size = atoi(strtok(NULL, " "));
    char **datasets = malloc(sizeof(char *) * (argc - 4));
    for (size_t i = 0; i < argc - 4; ++i) {
        datasets[i] = strtok(NULL, " ");
    }

    MnistTraining(sett, *network, datasets, argc - 4);
}

void callTesting(Network *network, size_t argc) {
    (void)argc;

    MnistTesting(*network, strtok(NULL, " "));
}

void testOnImage(Network *network, size_t argc) {
    (void)argc;

    SDL_Surface *img = SDL_Start(strtok(NULL, " "));
    Matrix input = image_to_matrix(img);

    int prediction = Predict(*network, input);
    printf("prediciton is: %d\n", prediction);
}

struct parseEl {
    char *cmd_name;
    size_t min_argc;
    void (*fct)(Network *network, size_t argc);
};

// don't forget to update CMD_COUNT when adding command
struct parseEl parseList[] = {
    {"help", 0, PrintUsage},    {"new", 0, CreateNetwork},
    {"train", 5, callTraining}, {"test", 1, callTesting},
    {"save", 1, callSave},      {"load", 1, callLoad},
    {"img", 1, testOnImage}};

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
        if (getline(&input, &len, stdin) <= 1)
            continue;

        // remove trailing newline from user inputs
        input[strcspn(input, "\n")] = 0;

        // get number of args and command name
        size_t argc = GetArgc(input);
        char *cmd_name = strtok(input, " ");

        // exit if requested
        if (strcmp(cmd_name, "quit") == 0)
            break;

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
    freeNetwork(*network);
    printf("Goodbye!\n");
    return 0;
}
