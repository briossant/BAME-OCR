CC=gcc
CFLAGS=-W -Wall -Wextra -O0 -g `pkg-config sdl2 --cflags`
LDFLAGS= `pkg-config sdl2 --libs` -lSDL2_image -lm 
EXEC=ImageProcess
SRC= $(wildcard *.c)
OBJ= $(SRC:.c=.o)


all: $(EXEC)

ImageProcess: $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -f ImageProcess *.o *.d

