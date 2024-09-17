# ##################### #
#   Demo: Input         #
# Author: Javier Lozano #
# ##################### #

# Variables

CC   := cc
OPT  := -Wall
INC  := `sdl2-config --cflags`
LIB  := -lSDL2
PROJ := input

# Rules

all: $(PROJ)

debug: OPT += -g
debug: $(PROJ)

$(PROJ): main.o input.o
	$(CC) $(OPT) $^ -o $@ $(LIB)
main.o: main.c
	$(CC) $(OPT) $(INC) -c $<
input.o: input.c input.h
	$(CC) $(OPT) $(INC) -c $<

# Clean

.PHONY: clean
clean:
	@rm -fv *.o
	@rm -fv $(PROJ)

