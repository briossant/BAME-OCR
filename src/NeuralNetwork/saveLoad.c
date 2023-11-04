#include "NeuralNetwork.h"
#include "matrices/matrices.h"
#include "precision.h"
#include <iso646.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
NNValue *matrix_to_array(Matrix mat) {
    NNValue *array = (NNValue *)malloc(sizeof(NNValue) * (mat.h * mat.w));
    for (size_t i = 0; i < mat.w; ++i) {
        for (size_t j = 0; j < mat.h; j++) {
            array[i * mat.h + j] = mat.mat[i][j];
        }
    }
    return array;
}

Matrix array_to_matrix(double *array, size_t w, size_t h) {
    Matrix copy = MatInit(w, h, 0, "FromLoad");
    int k = 0;
    for (size_t i = 0; i < w; i++) {
        for (size_t j = 0; j < h; j++) {
            copy.mat[i][j] = array[k];
            k++;
        }
    }

    return copy;
}
void SaveNetwork(Network network, char *filepath) {
    FILE *file =
        fopen(filepath,
              "w"); // this will clear the file so we can write on it,
                    // if we don't want the
                    // file to be cleared do: fopen(filepath,"a") for appending

    if (file == NULL) {
        printf("Error this file does not exist\n");
        exit(1);
    }

    fprintf(file, "NN** %ld %ld \n", network.input_size, network.depth);
    for (size_t i = 0; i < network.depth; i++) {
        fprintf(file, "Layer %ld\nLayer Size: %ld\nWeights: ", i,
                network.layers[i].size);
        NNValue *weights = matrix_to_array(network.layers[i].weights);
        for (size_t k = 0;
             k < network.layers[i].weights.w * network.layers[i].weights.h;
             k++) {
            fprintf(file, "%lf ", weights[k]);
        }
        free(weights);
        fprintf(file, "\nBiases: ");
        for (size_t k = 0; k < network.layers[i].biases.h; k++) {
            fprintf(file, "%lf ", network.layers[i].biases.mat[0][k]);
        }

        fprintf(file, "\n");
    }
    fflush(file);
    fclose(file);
}

int isDigit(const char *str) {
    char *endptr;
    double result = strtod(str, &endptr);
    if (endptr != str &&
        (*endptr == '\0' || (*endptr == '\n' && endptr[1] == '\0'))) {
        return 1; // Valid double
    }
    return 0; // Not a valid double
}

double *get_numbers(char *str, int *size) {
    char *token = strtok(str, " ");
    double *numbers = NULL;
    int count = 0;

    while (token != NULL) {
        if (isDigit(token) && strcmp(token, "\n") != 0) {
            // Convert the token to a double and store it in the +++numbers
            // array
            double value = atof(token);
            numbers = (double *)realloc(numbers, sizeof(double) * (count + 1));
            numbers[count] = value;
            printf("%lf\n", value);
            count++;
        }
        token = strtok(NULL, " ");
    }
    *size = count;
    printf("done\n");
    return numbers;
}

void printformat(void) {
    printf(
        "Format: Neural Network Document\n\nThe format for the Neural Network "
        "Document is as follows:\n\nNN** <size> <depth>\n- NN** represents the "
        "neural network version.\n- <size> represents the size of the "
        "network.\n- <depth> represents the depth of the network.\n\nLayer "
        "Information:\n- For each layer:\n  - Layer Number: <layer number>\n  "
        "- Layer Size: <size of layer>\n  - Weights: <list of weights>\n  - "
        "Biases: <list of biases>\n\nExample:\n\nNN** 2 2\nLayer Number: "
        "0\nLayer Size: 4\nWeights: -0.226563 -0.778394 0.844753 0.991776 "
        "-0.351602 -0.969908 -0.249897 -0.933591\nBiases: -0.226563 -0.778394 "
        "0.844753 0.991776 -0.351602 -0.969908 -0.249897 -0.933591\n\nLayer "
        "Number: 1\nLayer Size: 2\nWeights: -0.519999 0.739942 -0.320166 "
        "0.756588 -0.802945 -0.702889 -0.955374 -0.005229\nBiases: -0.519999 "
        "0.739942 -0.320166 0.756588 -0.802945 -0.702889 -0.955374 -0.005229\n "
        "and so on");
}

Network LoadNetwork(char *filepath) {

    FILE *file = fopen(filepath, "r");

    if (file == NULL) {
        printf("Error this file does not exist or can't be opened\n");
        exit(1);
    }
    char line[200];
    if (fgets(line, 200, file) == NULL) {
        printf("The file was empty\n");
        exit(1);
    }
    int size = 0;
    double *numbers = get_numbers(line, &size);
    if (size != 2) {
        printf("There was not enough parameters for the depth and input, or "
               "way too much for the network\n");
        printformat();
        exit(1);
    }
    double input_size = numbers[0];
    double depth_layer = numbers[1];
    Layer *layers = (Layer *)malloc(sizeof(Layer) * depth_layer);
    while (fgets(line, 200, file) != NULL) {
        double number_of_layer = get_numbers(line, &size)[0];
        if (fgets(line, 200, file) == NULL) {
            printf("fgets failed or reached end of file\n");
            exit(1);
        }
        double size_of_layer = get_numbers(line, &size)[0];
        if (fgets(line, 200, file) == NULL) {
            printf("error on weights");
            exit(1);
        }

        Matrix weights =
            array_to_matrix(get_numbers(line, &size),
                            (int)size / (int)depth_layer, depth_layer);
        MatPrint(weights);
        printf("%d size for weights, %d\n", size, (int)size / (int)depth_layer);
        if (fgets(line, 200, file) == NULL) {
            printf("error on biases");
            exit(1);
        }
        Matrix biases =
            array_to_matrix(get_numbers(line, &size), depth_layer, 1);
        Layer layer;
        {
            layer.biases = biases;
            layer.weights = weights;
            layer.size = size_of_layer;
        }
        layers[(int)number_of_layer] = layer;
    }
    Network network = {// this is the new Network declaration
                       .input_size = input_size,
                       .depth = depth_layer,
                       .layers = layers};
    fflush(file);
    fclose(file);
    free(numbers);
    SaveNetwork(network, "loaded.txt");
    return network;
}
