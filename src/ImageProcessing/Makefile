CC=gcc
CFLAGS=-W -Wall -Wextra -O0 -g
LDFLAGS= -lSDL2 -lGL -lSDL2_image -lm 
EXEC=ImageProcess
SRC= $(wildcard *.c)
OBJ= $(SRC:.c=.o)


all: $(EXEC)

test: $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -f ImageProcess *.o *.d

