CC = gcc -Iinclude/

CFLAGS = -Wall -Wextra -std=c99 -O1 -g -fsanitize=address 
CPPFLAGS = `--cflags SDL2`
LDLFLAGS = -lSDL
LDLIBS = `SDL2`

BUILD := build
SOURCE_DIR := src

SRC = $(shell find $(SOURCE_DIR) -name "*.c" ! -name "main.c")
OBJ = $(SRC:%.c=$(BUILD)/%.o)
DEP = $(SRC:%.c=$(BUILD)/%.d)

all: init main clear

clear:
	find . -type d -empty -delete

init: 
	$(shell mkdir -p $(BUILD))
	$(shell mkdir -p $(SRC:%.c=$(BUILD)/%))

main: $(OBJ)
	gcc -o $@ $(CFLAGS) $^ $(LDLFLAGS) $(LDLIBS)

sudoku:
	make -C Sudoku_Solver

$(BUILD)/%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@ $(LDLFLAGS) $(LDLIBS) $(LDLFLAGS)

# Clean all trash files
clean:
	rm -rf $(BUILD)
	${RM} ${OBJ} ${DEP} main
	clear
