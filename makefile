CFLAGS = -Wall -Wextra -Iexternal -I.
LDFLAGS = -Lbuild -lglfw -lglad -lm

all: build external build/main run

run:
	@build/main

build/main: ./main.c ./renderer.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

build:
	mkdir $@

external: build/libglad.a

build/libglad.a: external/glad/glad.c
	$(CC) $(CFLAGS) -c -o build/glad.o $^
	ar -rc $@ build/glad.o
	rm build/glad.o

