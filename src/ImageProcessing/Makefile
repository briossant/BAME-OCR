CC=gcc
CFLAGS=-W -Wall -Wextra  
LDFLAGS= -lSDL2 -lGL -lSDL2_image
EXEC=test
SRC= $(wildcard *.c)
OBJ= $(SRC:.c=.o)


all: $(EXEC)

test: $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -f test *.o *.d

