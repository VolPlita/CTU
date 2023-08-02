CC := g++
CFLAGS := -std=c++17 -Wall -pedantic -Wextra -fsanitize=leak -g -O3 #-fsanitize=address #-g
LIBS := -lSDL2 -lSDL2_image -lSDL2_ttf -lstdc++fs

# Directories
SRCDIR := src
OBJDIR := obj
BINDIR := bin
DOCDIR := doc

# Header and source files
HDRS := $(addprefix $(SRCDIR)/, define.hpp Text.hpp Resources.hpp Buildings.hpp Tiles.hpp Buttons.hpp Menu.hpp Parser.hpp Game.hpp Actions.hpp)
SRCS := $(addprefix $(SRCDIR)/, main.cpp Text.cpp Resources.cpp Buildings.cpp Tiles.cpp Buttons.cpp Menu.cpp Parser.cpp Game.cpp Actions.cpp)
OBJS := $(SRCS:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)
EXECUTABLE := plitavol

# Default target
all: clean compile

# Build rule
compile: $(EXECUTABLE)

$(EXECUTABLE): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

# Object file rule
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp $(HDRS)
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Run target
run:
	./$(EXECUTABLE)

# Clean rule
clean:
	rm -rf $(OBJDIR) $(BINDIR) $(EXECUTABLE)

# Documentation target
doc: Doxyfile $(HDRS) $(SRCS)
	@echo "Generating documentation..."
	@doxygen Doxyfile
	mv doc/html/*  doc/

.PHONY: all compile run clean doc
