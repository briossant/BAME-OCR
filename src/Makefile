CC=gcc
CFLAGS=-W -Wall -Wextra -O3 `pkg-config sdl2 --cflags`
LDFLAGS= `pkg-config sdl2 --libs` -lSDL2_image -lm
EXEC=BAME
SRC_NN= $(wildcard NeuralNetwork/utilities/*.c) $(wildcard NeuralNetwork/matrices/*.c) $(wildcard NeuralNetwork/mnistHandler/*.c) $(wildcard NeuralNetwork/network/*.c) 
SRC_IP= $(filter-out ImageProcessing/ImageProcess.c, $(wildcard ImageProcessing/*.c))
SRC_SS= SudokuSolver/PrintGrid.c SudokuSolver/Sudoku_Solver.c
OBJ= $(SRC_NN:.c=.o) $(SRC_IP:.c=.o) $(SRC_SS:.c=.o)


all: $(EXEC)

BAME: $(OBJ) BAME.o
	$(CC) -o BAME.o -c BAME.c $(CFLAGS)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -f BAME *.o *.d */*.d */*.o */*/*.d */*/*.o


