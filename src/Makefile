CC=gcc
CFLAGS=-W -Wall -Wextra -O3 `pkg-config sdl2 --cflags` `pkg-config --cflags gtk+-3.0` -pthread
LDFLAGS= `pkg-config sdl2 --libs` -lSDL2_image -lm `pkg-config --libs gtk+-3.0`
EXEC=BAME
SRC_NN= $(wildcard NeuralNetwork/utilities/*.c) $(wildcard NeuralNetwork/matrices/*.c) $(wildcard NeuralNetwork/mnistHandler/*.c) $(wildcard NeuralNetwork/network/*.c) 
SRC_IP= $(filter-out ImageProcessing/ImageProcess.c, $(wildcard ImageProcessing/*.c))
SRC_SS= SudokuSolver/PrintGrid.c SudokuSolver/Sudoku_Solver.c
# SRC_IF= Interface/interface.c
OBJ= $(SRC_NN:.c=.o) $(SRC_IP:.c=.o) $(SRC_SS:.c=.o) BAME.o


all: $(EXEC)

BAME: $(OBJ) main.o
	$(CC) -o main.o -c main.c $(CFLAGS)
	$(CC) -o $@ $^ $(LDFLAGS)

interface: $(OBJ) Interface/interface.o
	$(CC) -o Interface/interface.o -c Interface/interface.c $(CFLAGS)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -f BAME *.o *.d */*.d */*.o */*/*.d */*/*.o


