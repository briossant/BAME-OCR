#include "printers.h"
#include "NeuralNetwork.h"

void PrintUsage(void) {
    printf("Usage:\n");
    printf("〉new [input layer] [hidden layer] ... [output layer]: create a "
           "new network\n");

    printf("END\n");
}

int main(void) {
    PrintTitle();
    printf("Type \"help\" for more information.\n\n");
    Network network;
    while (42) {
        char *line = NULL;
        size_t len = 0;
        ssize_t lineSize = 0;

        printf("〉");
        lineSize = getline(&line, &len, stdin);

        printf("You entered %s, which has %zu chars.\n", line, lineSize - 1);

        free(line);
    }
    return 0;
}
