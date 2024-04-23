# SDL2 Demos
# OpengGL Basic

CC = cc
OPT = -Wall

all: triangle rectangle

triangle: triangle.c
	$(CC) $(OPT) triangle.c external/glad/src/glad.c -o prog_$@ -I external/glad/include -lSDL2 -lGL

rectangle: rectangle.c
	$(CC) $(OPT) rectangle.c external/glad/src/glad.c -o prog_$@ -I external/glad/include -lSDL2 -lGL

