CC=gcc
CFLAGS=`sdl2-config --cflags`
LIBS=`sdl2-config --libs`

# path to sdl2
INCLUDE_PATHS=-I/opt/homebrew/Cellar/sdl2/2.30.0/include/SDL2

# target binary
TARGET=main

# source files
SRC=main.c

# compile the project
$(TARGET): $(SRC)
	$(CC) $(SRC) -o $(TARGET) $(CFLAGS) $(INCLUDE_PATHS) $(LIBS)

# clean build artifacts
clean:
	rm -f $(TARGET)
