.PHONY: main clean

GCC=g++
FLAGS=-std=c++14 -Wall -Werror
LINK=-lGLEW -lGL -lGLU -lglfw3 -lX11 -lXxf86vm -lXrandr -lpthread -lXi -ldl -lXinerama -lXcursor
LINKSOIL=-lSOIL -limage_helper -limage_DXT -lstb_image_aug

main: main.cpp
	$(GCC) $(FLAGS) -o $@ $< $(LINK) $(LINKSOIL)

run: main
	./main

clean:
	rm -f main

