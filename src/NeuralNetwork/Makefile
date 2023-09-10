CC=gcc
CFLAGS=-W -Wall -Wextra  
LDFLAGS= -lm
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

