CC=gcc
CFLAGS=$(shell sdl2-config --cflags)
LIBS=$(shell sdl2-config --libs) -lSDL2_ttf

INCLUDE_PATHS=-I/opt/homebrew/include/SDL2
LIBRARY_PATHS=-L/opt/homebrew/lib

# target binary
TARGET=main

# source files
SRC=main.c

# compile the project
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(SRC) -o $(TARGET) $(LIBS)

# clean build artifacts
clean:
	rm -f $(TARGET)