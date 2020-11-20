CC := g++
CCFLAGS += -l SDL2 -l SDL2_image -l SDL2_ttf -l SDL2_mixer

all: test.out

test.out: main.o rsdl.o
	$(CC) main.o rsdl.o $(CCFLAGS) -o test.out

main.o: main.cpp
	$(CC) -c main.cpp

rsdl.o: RSDL/src/rsdl.hpp RSDL/src/rsdl.cpp
	$(CC) -c RSDL/src/rsdl.cpp -o rsdl.o

.PHONY: clean
clean:
	rm -r *.o