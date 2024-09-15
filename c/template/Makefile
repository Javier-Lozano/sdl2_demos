# ##################### #
#   Demo: Template      #
# Author: Javier Lozano #
# ##################### #

# Variables
CC   = cc
OPT  = -Wall
INC  = `sdl2-config --cflags`
LIB  = -lSDL2
PROJ = template

# Rules

all: $(PROJ)

$(PROJ): template.o
	$(CC) $(OPT) $^ -o $@ $(LIB)

template.o: template.c
	$(CC) $(OPT) $(INC) -c template.c $(LIB)

.PHONY: clean
clean:
	rm -f *.o
	rm -f template
