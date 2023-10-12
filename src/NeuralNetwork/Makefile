CC=gcc
CFLAGS=-W -Wall -Wextra -Wpedantic -Wformat=2 -O3  
LDFLAGS= -lm -O3
EXEC=xor
SRC= $(wildcard *.c) 
OBJ= $(SRC:.c=.o)


all: $(EXEC)

xor: $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -f xor *.o *.d

