#include "NeuralNetwork.h"
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

double* get_numbers(const char* input, int* count) {
    double* numbers = NULL;
    *count = 0;
    double currentNumber = 0.0;
    int isParsingNumber = 0;
    int isParsingDecimal = 0;
    double decimalMultiplier = 0.1;

    for (int i = 0; input[i] != '\0'; i++) {
        if (isdigit(input[i])) {  //verify what libray is this and fix
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
                (*count)++;
                numbers = (double*)realloc(numbers, sizeof(double) * (*count));
                numbers[(*count) - 1] = currentNumber;
                currentNumber = 0.0;
                isParsingNumber = 0;
                isParsingDecimal = 0;
                decimalMultiplier = 0.1;
            }
        }
    }

    // Check for a number at the end of the string
    if (isParsingNumber) {
        (*count)++;
        numbers = (double*)realloc(numbers, sizeof(double) * (*count));
        numbers[(*count) - 1] = currentNumber;
    }

    return numbers;
}

Network LoadNetwork(char *filepath)
{

   FILE * file= fopen( filepath, "r"); // this will clear the file so we can write on it, if we don't want the 
                                       // file to be cleared do: fopen(filepath,"a" for appending
   //Initialize a Network
   //new Network net = Network(0,0)
   if (file == NULL)
   {
       printf ("Error this file does not exist or can't be opened\n");
       exit(1);
   }
   int l=0;
   if (fgets(line,l, file)!= NULL)
   {
       char* line = fgets(line,0, file);
       double* numbers = get_numbers(line);
        // input_size * depth layer 
        // D fois 
        // layer size L
        // node l fois
       //remember to free(numbers) every single time
       if ( sizeof(numbers) < 2)
       {
           printf ("there was no size or depth for the network");
           exit(1);
           //or do we take the usual values?
       }
       Network network = new Network(numbers[1], numbers[0]);  //verify
       l++;
       size_t D= numbers[1];
       while ( D>0 && fgets(line, l, file)!= NULL)
       {
            
            char* line = fgets(line,l, file);
            double* numbers = get_numbers(line);
            // new layer put layer size 
            size_t L= numbers[0];
            while (L>0 && fgets(line, l, file)!= NULL)
            {
                char* line = fgets(line,0, file);
                double* numbers = get_numbers(line);
                //create a node and put the weights
                L--;
                l++;
            }

            if (L!=0)
            {
                printf ("the file did not have all the nod's info, stoped at %ld node, %ld layer", L, D);
                exit(1);
            }
            D--;
            l++;
       }
       if (D!=0)
       {
           printf( "the file did not have all the Layers, stoped at %ld layer",D); 
           exit(1);
       }
    
   }
   else {
    return Network(0,0); // what is the default network??
    printf(" The default Network was Loaded");
   }

}

