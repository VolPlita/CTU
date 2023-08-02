CC := g++
CFLAGS := `sdl2-config --libs --cflags` -Wall -pedantic -Wextra -O3 #-fsanitize=leak -g
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
EXECUTABLE = $(BINDIR)/plitavol

# Default target
all: compile

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
	$(EXECUTABLE)

# Clean rule
clean:
	rm -rf $(OBJDIR) $(BINDIR)

# Documentation target
doc:
	doxygen Doxyfile

.PHONY: all compile run clean doc

# # CC := g++
# # CFLAGS := `sdl2-config --libs --cflags` -Wall -pedantic -Wextra -O3 -fsanitize=leak -g -lSDL2_ttf -lm #-fsanitize=address

# # EXEC := plitavol 

# # all: $(EXEC)

# # compile: expr.o main.o tree.o
# # 	$(CC) -o expr.exe expr.o main.o tree.o 

# # main.o: main.cpp tree.h expr.h
# # 	$(CC) $(CFLAGS) -c -o main.o main.cpp

# # tree.o: tree.cpp tree.h
# # 	$(CC) $(CFLAGS) -c -o tree.o tree.cpp

# # expr.o: expr.cpp expr.h tree.h
# # 	$(CC) $(CFLAGS) -c -o expr.o expr.cpp

# # clean:
# # 	rm -f expr.exe expr.o main.o tree.o

# CC := g++
# CFLAGS := `sdl2-config --libs --cflags` -Wall -pedantic -Wextra -O3 -fsanitize=leak -g -lSDL2_ttf -lstdc++fs -lm #-fsanitize=address

# HDRS := src/define.hpp src/Text.hpp src/Resources.hpp src/Buildings.hpp src/Tiles.hpp src/Buttons.hpp src/Menu.hpp src/Parser.hpp src/Game.hpp src/Actions.hpp
# SRCS := src/main.cpp src/Text.cpp src/Resources.cpp src/Buildings.cpp src/Tiles.cpp src/Buttons.cpp src/Menu.cpp src/Parser.cpp src/Game.cpp src/Actions.cpp

# OBJS := $(SRCS:.c=.o)

# EXEC := plitavol 

# compile: all

# all: $(EXEC)

# $(EXEC): $(OBJS) $(HDRS) Makefile
# 	$(CC) -o $@ $(OBJS) $(CFLAGS)

# run:
# 	./$(EXEC)

# clean:
# 	rm -f $(EXEC) $(OBJS)

# doc:


# .PHONY: all clean