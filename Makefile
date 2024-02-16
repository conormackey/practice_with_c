CC=gcc
CFLAGS=`sdl2-config --cflags`
LIBS=`sdl2-config --libs`

# Replace /path/to/SDL2/include if sdl2-config doesn't handle it automatically
INCLUDE_PATHS=-I/opt/homebrew/Cellar/sdl2/2.30.0/include/SDL2

# Define the target executable
TARGET=main

# Define source files
SRC=main.c

# Rule to make the target
$(TARGET): $(SRC)
	$(CC) $(SRC) -o $(TARGET) $(CFLAGS) $(INCLUDE_PATHS) $(LIBS)

# Rule for cleaning up
clean:
	rm -f $(TARGET)
