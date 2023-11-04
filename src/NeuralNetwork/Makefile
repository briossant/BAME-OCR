CC=gcc
CFLAGS=-W -Wall -Wextra -O3#-pg -g
LDFLAGS= -lm -O3 #-pg -g
EXEC=xor
SRC= $(wildcard utilities/*.c) $(wildcard matrices/*.c) $(wildcard mnistHandler/*.c) $(wildcard network/*.c) 
OBJ= $(SRC:.c=.o)


all: $(EXEC)

xor: $(OBJ) xor.o
	$(CC) -o xor.o -c xor.c $(CFLAGS)
	$(CC) -o $@ $^ $(LDFLAGS)

mnist: $(OBJ) mnist.o
	$(CC) -o mnist.o -c mnist.c $(CFLAGS)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -f xor mnist *.o *.d

