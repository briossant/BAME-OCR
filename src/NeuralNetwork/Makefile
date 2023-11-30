CC=gcc
CFLAGS=-W -Wall -Wextra -O3 `pkg-config sdl2 --cflags`
LDFLAGS= -lm -O3 `pkg-config sdl2 --libs` -lSDL2_image
EXEC=mnist
SRC= $(wildcard utilities/*.c) $(wildcard matrices/*.c) $(wildcard mnistHandler/*.c) $(wildcard network/*.c) 
OBJ= $(SRC:.c=.o)


all: $(EXEC)

mnist: $(OBJ) mnist.o
	$(CC) -o mnist.o -c mnist.c $(CFLAGS)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -f xor mnist *.o *.d */*.d */*.o

