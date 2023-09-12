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
   
   fprintf(file, "Neural Network-- %ld %ld \n", network.input_size,network.depth);
   for ( size_t i =0; i< network.depth; i++)
   {
      fprintf(file, "Layer size--%ld \n", network.layers[i].size );
      for(size_t j=0; j< network.layers[i].size; j++)
      {
         Node nd = network.layers[i].nodes[j];
         fprintf(file,"Node-- %ld %f \n", nd.weight_size, nd.bias);
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

Network LoadNetwork(char *filepath);

