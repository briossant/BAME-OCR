#include <stdio.h>
#include <stdlib.h>


Network LoadNetwork(char *filepath) {
    FILE *file = fopen(filepath, "r");

    if (file == NULL) {
        printf("Error: This file does not exist or can't be opened\n");
        exit(1);
    }

    // Determine the file size
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Read the entire file into a buffer
    char *buffer = (char *)malloc(file_size + 1);
    if (buffer == NULL) {
        printf("Error: Memory allocation failed\n");
        exit(1);
    }

    fread(buffer, 1, file_size, file);
    buffer[file_size] = '\0';  // Null-terminate the buffer

    fclose(file);

    // Now parse the content in the buffer
    char *line = buffer;

    int size = 0;
    double *numbers = get_numbers(line, &size);

    // Extract input_size and depth_layer
    if (size != 2) {
        printf("There was not enough parameters for the depth and input, or way too much for the network\n");
        // Handle error as needed
        exit(1);
    }

    size_t input_size = (size_t)numbers[0];
    size_t depth_layer = (size_t)numbers[1];
    Layer *layers = (Layer *)malloc(sizeof(Layer) * depth_layer);

    size_t previous_layer_size = input_size;

    char *endptr;
    for (size_t i = 0; i < depth_layer; ++i) {
        // Extract layer_index
  //  ---> Not in includes?    int layer_index = strtol(line, &endptr, 10);
        if (endptr == line) {
            printf("Error parsing layer_index\n");
            exit(1);
        }

        // Move to the next line
        line = endptr;

        // Extract weights
        double *weights_numbers = get_numbers(line, &size);
        Matrix weights = array_to_matrix(weights_numbers, previous_layer_size, size);

        // Move to the next line
        line = strchr(line, '\n') + 1;

        // Extract biases
        double *biases_numbers = get_numbers(line, &size);
        Matrix biases = array_to_matrix(biases_numbers, 1, size);

        // Move to the next line
        line = strchr(line, '\n') + 1;

        // Create Layer and store in layers array
        Layer layer = {
            .biases = biases,
            .weights = weights,
            .size = size
        };

        layers[layer_index] = layer;
        previous_layer_size = size;
    }

    // Clean up and return the network
    free(buffer);
    // ...

    Network network = {
        .input_size = input_size,
        .depth = depth_layer
    }
}
