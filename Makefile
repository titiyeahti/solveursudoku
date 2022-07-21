CFLAGS=-O3 -ANSI -Wall -Wextra

all: main

main: solveur.o 

%.o:

.PHONY: clean
clean:
  rm *.o main
