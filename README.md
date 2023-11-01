# BAME-OCR

An optical character recogniser and sudoku solver app. 

## Overview

The current version includes a neural network restricted to the XOR problem with
its CLI, an image processing unit with its CLI and a sudoku solver.

## Installation & Requirements

Just clone the master branch for setup.
The program is written in c and requires the SDL and SDL-image packages.
The program is only tested on linux with the gcc compiler.

## Usage

For the moment every step of the Sudoku solver is separated.
Run the application in your preferred terminal using the following command:

You have a Makefile you can use, with the commands clean and all. The Make
function on it's own will display the "help" menu on the Terminal for you to
know what arguments to use in function of what you wish to do.

The following arguments are recognised by the make command:
* 
*
*
*
*
### Output:

Outputs for the moment are independent, the outputs vary depending on the
function you execute. 
 
These should be the following outputs:

*
*
*
*
*

### Image processing

The image processing can be found under src/NeuralNetwork/ 

Run ```make``` to compile and then ```./ImageProcess``` to enter the CLI.
The program doesn't take any arguments.

CLI usage:

* -a, --all                   Apply all the filters for the OCR
* -i, --input                 Input file
* -o, --output                Output file
* -h, --help                  Display this help message
* -v, --version               Display version information
* -g, --greyscale             Convert the image to grayscale
* -bw, --blackwhite           Convert the image to black and white
* -c, --contrast              Adjust the image contrast
* -gb, --gaussianblur         Apply Gaussian blur to the image
* -b, --bright                Adjust the image brightness
* -ag, --arroundgaussianblur  Apply Gaussian blur to the image
* -r, --rotate                Rotate the image
* -ca, --canny                Apply Canny filters to the image

### XOR Neural Network

The neural network can be found under src/NeuralNetwork/ 

Run ```make``` to compile and then ```./xor``` to enter the CLI.
The program doesn't take any arguments.

CLI usage:

* 〉help : show this message
* 〉new [hidden layer size] ... : create a new network
* 〉train [learning rate] [epochs] [inertia strength] : train the network
* 〉run [0 or 1] [0 or 1] : run the prediction on those inputs
* 〉save [filename] : save the network
* 〉load [filename] : load the network


### Sudoku solver

...


## License 

This application is open-source and free for all users.


