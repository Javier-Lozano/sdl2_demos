# ##################### #
# Demo:   Input         #
# Author: Javier Lozano #
# ##################### #

# Variables

OPT = -Wall
LIB = -lSDL2

# All

all: input

# Rules

input: input.o main.o
	gcc $(OPT) $^ -o $@ $(LIB)

input.o: input.c input.h
	gcc $(OPT) -c input.c

main.o: main.c input.h
	gcc $(OPT) -c main.c

# Clean

clean:
	rm -f *.o
	rm -f input
