CC=gcc
CFLAGS=-W -Wall -Wextra -Wformat=2 -O3
LDFLAGS= -lm -O3
EXEC=xor
SRC= $(wildcard *.c) $(wildcard matrices/*.c) 
OBJ= $(SRC:.c=.o)


all: $(EXEC)

xor: $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -f xor *.o *.d

