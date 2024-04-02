CC=/opt/homebrew/Cellar/llvm/17.0.6_1/bin/clang

CFLAGS = -g -Wall -std=c99 -fopenmp
CXXFLAGS = -g -Wall -fopenmp
LDFLAGS = -fopenmp

all: lab1prog

lab1prog: cw4301.o
	$(CC) $(LDFLAGS) -o lab1prog cw4301.o

*.o: *.c
	$(CC) $(CFLAGS) -c %<.c -o $@.o

clean:
	rm -rf *.o lab1prog
