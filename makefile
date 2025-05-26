PROJECT = hanoi-visualizer

# Compiler
CC = g++
CFLAGS = -Wall -g -std=c++23
INCLUDE = -Iinc

# Platform
OS = $(shell uname -s 2>/dev/null || echo Windows)
ifeq ($(OS),Windows)
    BIN_SUFFIX = .exe
    RM = del /Q
    MKDIR = mkdir
    ROOT = $(subst /,\,$1)
else
    BIN_SUFFIX =
    RM = rm -f
    MKDIR = mkdir -p
    ROOT = $1
endif

# Platform-specific LDFLAGS for raylib
ifeq ($(OS),Windows)
    RAYLIB_LDFLAGS = -lraylib -lopengl32 -lgdi32 -lwinmm
else
    RAYLIB_LDFLAGS = -lraylib -lm -lpthread -ldl -lrt
endif

# Files
TARGET = dist/$(PROJECT)$(BIN_SUFFIX)
SOURCES = $(wildcard src/*.cpp)
OBJECTS = $(patsubst src/%.cpp,obj/%.o,$(SOURCES))

# Default
all: | obj_dir $(TARGET)

# Create object directory
obj_dir:
	@$(MKDIR) $(call ROOT,obj)

# Target
$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(call ROOT,$(TARGET)) $(RAYLIB_LDFLAGS)

# Object files
obj/%.o: src/%.cpp
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $(call ROOT,$@)

# Clean up
clean:
	@echo "Cleaning object files and targets..."
	$(RM) $(call ROOT,$(OBJECTS)) $(call ROOT,$(TARGET))

.PHONY: all obj_dir clean