CC=gcc
CFLAGS=$(shell sdl2-config --cflags)
LIBS=$(shell sdl2-config --libs) -lSDL2_ttf

INCLUDE_PATHS=-I/opt/homebrew/include/SDL2
LIBRARY_PATHS=-L/opt/homebrew/lib

# target binary
TARGET=main

# source files
SRC=main.c graphics.c utils.c text.c

# object files
OBJ=$(SRC:.c=.o)

# compile the project
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $^ -o $@ $(LIBS)

# generic rule for compiling .c to .o
%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDE_PATHS) -c $< -o $@

# clean build artifacts
clean:
	rm -f $(TARGET) $(OBJ)