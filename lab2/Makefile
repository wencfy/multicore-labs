CC=/opt/homebrew/Cellar/llvm/17.0.6_1/bin/clang

CFLAGS = -g -Wall -std=c99 -fopenmp
CXXFLAGS = -g -Wall -fopenmp
LDFLAGS = -fopenmp

all: ptsm

ptsm: cw4301.o
	$(CC) $(LDFLAGS) -o ptsm cw4301.o

*.o: *.c
	$(CC) $(CFLAGS) -c %<.c -o $@.o

clean:
	rm -rf *.o ptsm