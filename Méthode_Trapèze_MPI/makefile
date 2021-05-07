MCC = mpicc
CC = gcc
CCFLAGS = -lm 

all:  PiTrapeze

PiTrapeze: PiTrapeze.c
	${MCC} -o PiTrapeze  PiTrapeze.c ${CCFLAGS}

clean:
	rm PiTrapeze *.o

