CC := g++
CFLAGS := `sdl2-config --libs --cflags` -Wall -pedantic -Wextra -O3 -fsanitize=address -g -lSDL2_image -lm

HDRS := GameControll.hpp Buildings.hpp Map.hpp Events.hpp
SRCS := main.cpp GameControll.cpp Buildings.cpp Map.cpp Events.cpp

OBJS := $(SRCS:.c=.o)

EXEC := game 

all: $(EXEC)
 
showfont: showfont.c Makefile
	$(CC) -o $@ $@.c $(CFLAGS) $(LIBS)

glfont: glfont.c Makefile
	$(CC) -o $@ $@.c $(CFLAGS) $(LIBS)

$(EXEC): $(OBJS) $(HDRS) Makefile
	$(CC) -o $@ $(OBJS) $(CFLAGS)

clean:
	rm -f $(EXEC) $(OBJS)

.PHONY: all clean