#include "NeuralNetwork.h"
#include <iso646.h>
#include <stdio.h>
#include <stdlib.h>


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
   
   fprintf(file, "NN-- %ld %ld \n", network.input_size,network.depth);
   for ( size_t i =0; i< network.depth; i++)
   {
      fprintf(file, "L--%ld \n", network.layers[i].size );
      for(size_t j=0; j< network.layers[i].size; j++)
      {
         Node nd = network.layers[i].nodes[j];
         fprintf(file,"N-- %ld %f \n", nd.weight_size, nd.bias);
         for (size_t w=0; w<nd.weight_size; w++)
         {
             fprintf(file, "%f ", nd.weights[w]);
         }
         fprintf(file,"\n");
      }
      
   }
   fflush(file);
   fclose(file);

}

int isDigit(char c) {
    return c >= '0' && c <= '9';
}

double* get_numbers(const char* input) { // I think *count isn't needed as an argument
    double* numbers = NULL;
    int count = 0;
    double currentNumber = 0.0;
    int isParsingNumber = 0;
    int isParsingDecimal = 0;
    double decimalMultiplier = 0.1;

    for (int i = 0; input[i] != '\0'; i++) { 
        if (isDigit(input[i])) { // isDigit done  
            if (isParsingDecimal) {
                currentNumber = currentNumber + (input[i] - '0') * decimalMultiplier;
                decimalMultiplier /= 10.0;
            } else {
                currentNumber = currentNumber * 10 + (input[i] - '0');
            }
            isParsingNumber = 1;
        } else if (input[i] == '.' && isParsingNumber) {
            isParsingDecimal = 1;
        } else {
            if (isParsingNumber) {
                (count)++;
                numbers = (double*)realloc(numbers, sizeof(double) * (count));
                numbers[(count) - 1] = currentNumber;
                currentNumber = 0.0;
                isParsingNumber = 0;
                isParsingDecimal = 0;
                decimalMultiplier = 0.1;
            }
        }
    }

    // Check for a number at the end of the string
    if (isParsingNumber) {
        (count)++;
        numbers = (double*)realloc(numbers, sizeof(double) * (count));
        numbers[(count) - 1] = currentNumber;
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
    int l=0;
    char* line;

    fgets(line,l, file);

    if (line == NULL) { 
        printf("The file was empty\n");
        exit(1); 
    }       

    double* numbers = get_numbers(line);
    if ( sizeof(numbers) < 2)
    {
        printf ("there was no size or depth for the network\n"); 
        exit(1);
    }
    
    int input_size = numbers[0];
    int depth_layer= numbers[1]; 
    // D fois 
    // layer size L
    // node l fois
    //remember to free(numbers) every single time
    // -1?? beacuse the input layer isn't a proper layer  
    Layer *layers = malloc(sizeof(Layer) * numbers[depth_layer]); // the array needs to be created before
    
    size_t D = numbers[1];
    l++;
    while ( D>0 && fgets(line, l, file)!= NULL)
    {
        numbers = get_numbers(line);
        // new layer put layer size L 
        size_t layer_size = numbers[0];
        int i=0;
        l++;
        //create the new layer with layer size then put all the nods inside
        Node* nods = malloc ( sizeof (Node) * layer_size);
        while (layer_size>0 && fgets(line, l, file)!= NULL)
        {   
            int n=0;
            if (line != NULL)
            {
                numbers=get_numbers(line);
                int node_weight= (int)numbers[0];
                double node_bias= numbers[1];
                l++;
                double* weight = malloc(sizeof(double) * node_weight);
                if ( fgets(line, l , file)!= NULL)
                {
                    numbers= get_numbers(line);
                    for (int i=0; i<node_weight; i++)
                    {
                        weight[i]=numbers[i];
                    }
                }
                nods[n]=new_node(node_weight, weight, node_bias);
                n++;
                layer_size--;
                l++;
                
            }

            layers[i]=new_layer( nods, layer_size);
            i++;
        }
        

        if (layer_size!=0)
        {
            printf ("the file did not have all the nod's info, stoped at %ld node, %ld layer\n", layer_size, D);
            exit(1);
        }
        D--;
        l++;
    }
    if (D!=0)
    {
        printf( "the file did not have all the Layers, stoped at %ld layer\n",D); 
        exit(1);
    }
    
    Network network = { // this is the new Network declaration
        .input_size = input_size, 
        .depth = depth_layer,
        .layers = layers
    };  
    return network;
}

