#include "printers.h"
#include "NeuralNetwork.h"
#include <string.h>

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
        char *input = NULL;
        size_t len = 0;

        printf("〉");
        getline(&input, &len, stdin);
        input[strcspn(input, "\n")] = 0;

        char *ptr = strtok(input, " ");

        while (ptr != NULL) {
            printf("'%s'\n", ptr);
            ptr = strtok(NULL, " ");
        }
        free(input);
    }
    return 0;
}
