CC=mpicc
C_FLAGS=-Wall -pedantic -std=c99 -g
L_FLAGS=-lm -lrt
HOSTFILE=hosts
PROGRAM=concert

SOURCES=$(wildcard *.c)
BINARIES=$(SOURCES:%.c=%.c.o)

all: $(PROGRAM)

$(PROGRAM): $(BINARIES)
	$(CC) -o $@ $^ $(C_FLAGS)

%.c.o: %.c
	$(CC) -c -o $@ $< $(C_FLAGS)

.PHONY: clean run

compile: all

run: all
	mpirun -n $(n) ./${PROGRAM} $(plik)

debug: 
	mpirun -n $(n) --debug ./${PROGRAM} $(plik)

	
clean:
	-rm -f *.o $(PROGRAM)
