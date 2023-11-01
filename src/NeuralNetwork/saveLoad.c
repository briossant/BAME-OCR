#include "NeuralNetwork.h"
#include "matrices/matrices.h"
#include "precision.h"
#include <iso646.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BUFFER_SIZE 200;
NNValue* matrix_to_array(Matrix mat)
{
    NNValue* array = (NNValue *)malloc(sizeof(NNValue)*(mat.h*mat.w));
     for (size_t i = 0; i < mat.w; ++i) {
            for (size_t j = 0; j < mat.h; j++) {
                array[ i*mat.h + j ]= mat.mat[i][j];
            }
        }
     return array;
}

Matrix array_to_matrix(double* array, size_t w, size_t h) 
{
        Matrix copy = MatInit(w, h, 0.0, "FromLoad");
        for (size_t i = 0; i < w; i++) {
            for (size_t j = 0; j < h; j++)
                copy.mat[i][j]=array[ i*h + j ];
        }
   
        return copy;
}
void SaveNetwork(Network network, char* filepath)
{
   FILE * file= fopen( filepath, "w"); // this will clear the file so we can write on it, 
                                       // if we don't want the 
                                       // file to be cleared do: fopen(filepath,"a") for appending

   if (file == NULL)
   {
       printf ("Error this file does not exist\n");
       exit(1);
   }
   
   fprintf(file, "NN** %ld %ld \n", network.input_size,network.depth);
   for (size_t i=0; i<network.depth; i++)
   {
       fprintf(file,"Layer %ld\nLayer Size:%ld\nWeights:", i, network.layers[i].size);
        NNValue * weights = matrix_to_array(network.layers[i].weights);
        for (size_t k=0; k<network.layers[i].weights.w* network.layers[i].weights.h; k++)
        {
            fprintf(file, "%lf ",weights[k]);
        }
        free(weights);
            fprintf(file,"\nBiases:");
        NNValue * biases = matrix_to_array(network.layers[i].weights);
        for (size_t k=0; k<network.layers[i].weights.w* network.layers[i].weights.h; k++)
        {
            fprintf(file, "%lf ",biases[k]);
        }

        fprintf(file,"\n");
        free(biases);
   }
   fflush(file);
   fclose(file);

}
int isDigit(const char* str) {
    char* endptr;
    strtod(str, &endptr);
    if (*endptr != '\0' && *endptr != '\n') {
        return 0; // Not a valid double
    }
    return 1; // Valid double
   
}
   
double* get_numbers ( char* str)
{
    char* token = strtok(str, " ");
    double* numbers = NULL;
    int count = 0;
   
    while (token != NULL) {
        if (isDigit(token)) {
                // Convert the token to a double and store it in the            +++numbers array
            double value = atof(token);
            numbers = (double*)realloc(numbers, sizeof(double) *  (count + 1));
            numbers[count] = value;
            count++;
        }
        token = strtok(NULL, " ");
    }
    return numbers;
}

Network LoadNetwork(char *filepath)
{

    FILE * file= fopen( filepath, "r");  
   
    if (file == NULL)
    {
        printf ("Error this file does not exist or can't be opened\n");
        exit(1);
    }
    char line[200];
    if (fgets(line, 200, file)== NULL) { 
        printf("The file was empty\n");
        exit(1); 
    }

    double* numbers =get_numbers(line);
    if (sizeof(numbers)<2)
    {
        printf("There was not enough parameters for the depth and input, or way too much for the network");
        exit(1);
    }
    double input_size= numbers[0];
    double depth_layer=numbers[1];
    Layer* layers = (Layer*)malloc(sizeof(Layer)*depth_layer); 
    while (fgets(line, 200, file)!= NULL) { 
        double* number_of_layer =get_numbers(line);
        double* size_of_layer =get_numbers(fgets(line,200,file));
        if (fgets(line, 200, file)==NULL) 
        {
            printf("error on weights");
            exit(1);
        }
        Matrix weights =array_to_matrix(get_numbers(line), size_of_layer[0],depth_layer );
        if (fgets(line, 200, file)==NULL) 
        {
            printf("error on biases");
            exit(1);
        }
        Matrix biases =array_to_matrix(get_numbers(line), 1,depth_layer );
        Layer layer;{
        layer.biases=biases;
        layer.weights=weights;
        layer.size= size_of_layer[0];
        }
        layers[(int)number_of_layer[0]]=layer;
    }
    Network network = { // this is the new Network declaration
        .input_size =input_size, 
        .depth = depth_layer,
        .layers = layers
    };  
    return network;

}

