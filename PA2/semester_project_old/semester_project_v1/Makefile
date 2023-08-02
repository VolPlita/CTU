CC := g++
CFLAGS := `sdl2-config --libs --cflags` -Wall -pedantic -Wextra -O3 -fsanitize=leak -g -lSDL2_ttf -lm #-fsanitize=address

HDRS := src/define.hpp src/Text.hpp src/Resources.hpp src/Buildings.hpp src/Tiles.hpp src/Buttons.hpp src/Menu.hpp src/Parser.hpp src/Game.hpp
SRCS := src/tile2.cpp src/Text.cpp src/Resources.cpp src/Buildings.cpp src/Tiles.cpp src/Buttons.cpp src/Menu.cpp src/Parser.cpp src/Game.cpp

OBJS := $(SRCS:.c=.o)

EXEC := game 

all: $(EXEC)

$(EXEC): $(OBJS) $(HDRS) Makefile
	$(CC) -o $@ $(OBJS) $(CFLAGS)

clean:
	rm -f $(EXEC) $(OBJS)

.PHONY: all clean