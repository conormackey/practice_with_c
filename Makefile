CC=gcc
# Fetch compiler flags for SDL2
CFLAGS=$(shell sdl2-config --cflags)
# Fetch linker flags for SDL2 and add SDL2_ttf
LIBS=$(shell sdl2-config --libs) -lSDL2_ttf

# Assuming SDL2 and SDL2_ttf are installed in standard locations via Homebrew or similar
# If SDL2_ttf is not in a standard location, you might need to adjust INCLUDE_PATHS and LIBRARY_PATHS accordingly
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