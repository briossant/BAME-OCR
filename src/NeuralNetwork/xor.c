#include "printers.h"
#include "NeuralNetwork.h"
#include <string.h>

#define CMD_COUNT 1

void PrintUsage(Network *network) {
    printf("Usage:\n");
    printf("〉new [input layer] [hidden layer] ... [output layer]: create a "
           "new network\n");

    printf("END\n");
}

size_t GetArgc(char *input) {
    char *p = input;
    size_t res = 0;
    while (*p != 0)
        if (*(p++) == ' ')
            ++res;
    return res;
}

struct parseEl {
    char *cmd_name;
    size_t min_argc;
    void (*fct)(Network *network);
};

struct parseEl parseList[] = {{"help", 0, PrintUsage}};

int main(void) {
    PrintTitle();
    printf("Type \"help\" for more information.\n\n");
    Network network;
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
            parseList[i].fct(&network);

        free(input);
    }
    return 0;
}
