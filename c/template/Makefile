# ##################### #
#   Demo: Template      #
# Author: Javier Lozano #
# ##################### #

# Variables

CC   := cc
OPT  := -Wall
INC  := `sdl2-config --cflags`
LIB  := -lSDL2
PROJ := template

# Rules

all: $(PROJ)

debug: OPT += -g
debug: $(PROJ)

$(PROJ): main.o
	$(CC) $(OPT) $^ -o $@ $(LIB)
main.o: main.c
	$(CC) $(OPT) $(INC) -c $<

# Clean

.PHONY: clean
clean:
	@rm -fv *.o
	@rm -fv $(PROJ)

