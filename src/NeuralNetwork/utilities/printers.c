#include "../network/NeuralNetwork.h"

void PrintSettings(TrainingSettings settings) {
    size_t nbr_of_batch = settings.nbr_of_inputs / settings.batch_size;
    printf(
        "〉⎧⏵⏵⏵ SETTINGS\n〉⎬> nbr_of_batch: %ld\n〉⎬> nbr_of_inputs: %ld \n"
        "〉⎬> batch_size: %ld\n"
        "〉⎬> training_rate: %f\n〉⎬> epochs: %ld \n〉⎬> inertia_strength: %f\n"
        "〉⎩⏴⏴⏴ SETTINGS END\n\n",
        nbr_of_batch, settings.nbr_of_inputs, settings.batch_size,
        settings.training_rate, settings.epochs, settings.inertia_strength);
}

void PrintTitle() {
    printf(
        " ▄▄▄▄    ▄▄▄       ███▄ ▄███▓▓█████     ▒█████   ▄████▄   ██▀███  \n");
    printf(
        "▓█████▄ ▒████▄    ▓██▒▀█▀ ██▒▓█   ▀    ▒██▒  ██▒▒██▀ ▀█  ▓██ ▒ ██▒\n");
    printf(
        "▒██▒ ▄██▒██  ▀█▄  ▓██    ▓██░▒███      ▒██░  ██▒▒▓█    ▄ ▓██ ░▄█ ▒\n");
    printf(
        "▒██░█▀  ░██▄▄▄▄██ ▒██    ▒██ ▒▓█  ▄    ▒██   ██░▒▓▓▄ ▄██▒▒██▀▀█▄  \n");
    printf(
        "░▓█  ▀█▓ ▓█   ▓██▒▒██▒   ░██▒░▒████▒   ░ ████▓▒░▒ ▓███▀ ░░██▓ ▒██▒\n");
    printf(
        "░▒▓███▀▒ ▒▒   ▓▒█░░ ▒░   ░  ░░░ ▒░ ░   ░ ▒░▒░▒░ ░ ░▒ ▒  ░░ ▒▓ ░▒▓░\n");
    printf(
        "▒░▒   ░   ▒   ▒▒ ░░  ░      ░ ░ ░  ░     ░ ▒ ▒░   ░  ▒     ░▒ ░ ▒░\n");
    printf(
        " ░    ░   ░   ▒   ░      ░      ░      ░ ░ ░ ▒  ░          ░░   ░ \n");
    printf(
        " ░            ░  ░       ░      ░  ░       ░ ░  ░ ░         ░     \n");
    printf("      ░                                         ░                "
           "\n\n\n");
}

void printNetwork(Network network) {
    printf("〉⎧⏵⏵⏵ NETWORK 🕸\n");
    printf("〉⎬> Settings: depth: %ld | input_size: %ld \n〉⎬> Layers:\n",
           network.depth, network.input_size);
    for (size_t i = 0; i < network.depth; i++) {
        printf("〉⎬⎼[%ld]> size: %ld \n", i, network.layers[i].size);
    }
    printf("〉⎩⏴⏴⏴ NETWORK END\n\n");
}
