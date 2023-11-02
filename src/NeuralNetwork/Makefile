CC=gcc
CFLAGS=-W -Wall -Wextra -O3#-pg -g
LDFLAGS= -lm -O3 #-pg -g
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

