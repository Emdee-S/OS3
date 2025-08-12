# Makefile for Memory Allocation Simulation Program (C Version)
# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -g

# Target executable
TARGET = sim

# Source files
SOURCES = main.c mem_component.c stats.c

# Object files
OBJECTS = $(SOURCES:.c=.o)

# Header files
HEADERS = mem_component.h stats.h

# Default target
all: $(TARGET)

# Link the executable
$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET)

# Compile source files to object files
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build files
clean:
	del /Q $(OBJECTS) $(TARGET) 2>nul

# Run the program
run: $(TARGET)
	$(TARGET).exe

# Phony targets
.PHONY: all clean run
