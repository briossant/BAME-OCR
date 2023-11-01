# BAME-OCR

An optical character recogniser and sudoku solver app. 

## Overview

The current version includes a neural network restricted to the XOR problem with
its CLI, an image processing unit with its CLI and a sudoku solver.

## Installation & Requirements

Just clone the master branch for setup.

The program is written in c and requires the SDL2 and SDL2-image packages.

The program is only tested on linux with the gcc compiler.

## Usage

Since the project isn't finished, only the sub-program can be used separatly.
Each of them can be found under ```src/[sub-program name]/```.

Details on those below.

### Image processing

The image processing can be found under ```src/ImageProcessing/```

Run ```make``` to compile and then ```./ImageProcess``` to enter the CLI.
The program doesn't take any arguments.

CLI usage:

* a, all: Apply all the filters for the OCR
* i, input [filename]: Input file
* o, output [filename]: Output file
* h, help: Display this help message
* v, version: Display version information
* g, greyscale: Convert the image to grayscale
* bw, blackwhite: Convert the image to black and white
* c, contrast: Adjust the image contrast
* gb, gaussianblur: Apply Gaussian blur to the image
* b, bright: Adjust the image brightness
* ag, arroundgaussianblur: Apply Gaussian blur to the image
* r, rotate [angle]: Rotate the image
* ca, canny: Apply Canny filters to the image

### XOR Neural Network

The neural network can be found under ```src/NeuralNetwork/```

Run ```make``` to compile and then ```./xor``` to enter the CLI.
The program doesn't take any arguments.

CLI usage:

* 〉 help : show this message
* 〉 new [hidden layer size] ... : create a new network
* 〉 train [learning rate] [epochs] [inertia strength]: train the network
* 〉 run [0 or 1] [0 or 1]: run the prediction on those inputs
* 〉 save [filename]: save the network
* 〉 load [filename]: load the network


### Sudoku solver

The Sudoku solver can be found undre ```src/SudokuSolver```

Run ```make``` to compile and then use ```./solver <grid>``` to solve the grid

The grid file need to be a texte file, with extension or not, as you choose.

The solver only accept to format of grid. The format like ```grid_00``` and ```grid_01```
The solver print the original grid without modification and solution and it print the solved grid

It doesn't save the solved grid

## License 

This application is open-source and free for all users.


