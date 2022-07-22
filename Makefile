CFLAGS=-O3 -Wall -Wextra 

all: main

main: solveur.o 

%.o:

.PHONY: clean
clean:
	rm *.o main
