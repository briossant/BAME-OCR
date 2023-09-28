CC=gcc
CFLAGS=-W -Wall -Wextra -O0 -g
LDFLAGS= -lSDL2 -lGL -lSDL2_image -lm 
SRC_DIR = src
OBJ_DIR = obj

# Listez les fichiers source dans le sous-dossier
SOURCES = $(wildcard $(SRC_DIR)/*.c) $(wildcard $(SRC_DIR)/ImageProcessing/*.c)

# Générez la liste des fichiers objets correspondants
OBJECTS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SOURCES))

# Le nom de l'exécutable à générer
TARGET = ocr

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJECTS)

.PHONY: clean
