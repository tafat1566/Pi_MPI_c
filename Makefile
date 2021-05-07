MCC = mpicc
CC = gcc
CCFLAGS = -lm 

all:  MPI_MonteCarlo pi_client_serveur

MPI_MonteCarlo: MPI_MonteCarlo.c
	${MCC} -o MPI_MonteCarlo MPI_MonteCarlo.c ${CCFLAGS}
	
pi_client_serveur: pi_client_serveur.c
	${MCC} -o pi_client_serveur pi_client_serveur.c ${CCFLAGS}
	
clean:
	rm MPI_MonteCarlo pi_client_serveur *.o

