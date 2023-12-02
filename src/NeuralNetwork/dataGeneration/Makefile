CC=gcc
CFLAGS=-W -Wall -Wextra -g `pkg-config sdl2 --cflags`
LDFLAGS= -lm -g `pkg-config sdl2 --libs` -lSDL2_image
EXEC=convertor
SRC= ../../ImageProcessing/utilities.c convertToMnistFormat.c
OBJ= $(SRC:.c=.o)


all: $(EXEC)

convertor: $(OBJ) 
	$(CC) -o $@ $^ $(LDFLAGS)


clean:
	rm -f convertor *.o *.d ../../ImageProcessing/*.d ../../ImageProcessing/*.o

