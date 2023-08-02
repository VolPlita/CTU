# CC := g++
# CFLAGS := `sdl2-config --libs --cflags` -Wall -pedantic -Wextra -O3 -fsanitize=address -g -lSDL2_ttf -lm

# HDRS := GameControll.hpp Buildings.hpp Map.hpp Events.hpp
# SRCS := main.cpp GameControll.cpp Buildings.cpp Map.cpp Events.cpp

# OBJS := $(SRCS:.c=.o)

# EXEC := game 

# all: $(EXEC)
 
# showfont: showfont.c Makefile
# 	$(CC) -o $@ $@.c $(CFLAGS) $(LIBS)

# glfont: glfont.c Makefile
# 	$(CC) -o $@ $@.c $(CFLAGS) $(LIBS)

# $(EXEC): $(OBJS) $(HDRS) Makefile
# 	$(CC) -o $@ $(OBJS) $(CFLAGS)

# clean:
# 	rm -f $(EXEC) $(OBJS)

# .PHONY: all clean

#OBJS specifies which files to compile as part of the project
OBJS = tile4.cpp #Text.hpp

#CC specifies which compiler we're using
CC = g++

#COMPILER_FLAGS specifies the additional compilation options we're using
# -w suppresses all warnings
COMPILER_FLAGS = -Wall -pedantic -Wextra -Wshadow -fsanitize=address -O2 -g

#LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = -lSDL2 -lSDL2_ttf

#OBJ_NAME specifies the name of our exectuable
OBJ_NAME = game

#This is the target that compiles our executable
all : $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)

run: all
	./game