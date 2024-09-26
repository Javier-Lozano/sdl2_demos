# ##################### #
#   Demo: Input         #
# Author: Javier Lozano #
# ##################### #

# Variables

CC      := cc
OPT     := -Wall
I_FLAGS := `sdl2-config --cflags`
L_FLAGS := -lSDL2 -lSDL2_ttf
PROJECT := input

# Rules

all: $(PROJECT)

debug: OPT += -g
debug: $(PROJECT)

$(PROJECT): main.o input.o init.o
	$(CC) $(OPT) $^ -o $@ $(L_FLAGS)
main.o: main.c main.h
	$(CC) $(OPT) $(I_FLAGS) -c $<
init.o: init.c main.h
	$(CC) $(OPT) $(I_FLAGS) -c $<
input.o: input.c input.h
	$(CC) $(OPT) $(I_FLAGS) -c $<

# Clean

.PHONY: clean
clean:
	@rm -fv *.o
	@rm -fv $(PROJECT)

