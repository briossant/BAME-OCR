CC = gcc
CFLAGS = `pkg-config --cflags gtk+-3.0` -g -Wall -Wextra 
LIBS = `pkg-config --libs gtk+-3.0`

SRCS = interface.c
OBJS = $(SRCS:.c=.o)
EXEC = interface

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(OBJS) -o $(EXEC) $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(EXEC)

