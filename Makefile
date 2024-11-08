# Variables
CC = gcc                  # Compiler
CFLAGS = -std=c11 -Wall    # Compiler options
LDFLAGS = -lraylib -lm -lpthread -ldl -lX11   # Flags for include Raylib

# Raylib routes
INCLUDE_DIR = /opt/homebrew/include
LIB_DIR = /opt/homebrew/lib

# Code files and Executables
SRC = src/main.c
OBJ = build/main.o         
EXE = build/pong           # Final executable

# Create directory build if it doesn't exist
$(shell mkdir -p build)

# Principal rule for create directory
$(EXE): $(OBJ)
	$(CC) -o $(EXE) $(OBJ) $(LDFLAGS) -L$(LIB_DIR)

# Rule for compilate
build/main.o: src/main.c
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

# Remove generated files
clean:
	rm -rf build

# Make command for execute game
run: $(EXE)
	./$(EXE)