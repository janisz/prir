#define _GNU_SOURCE 
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <errno.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "mpi.h"


int main(int argc, char* argv[])
{
int rank, size;


/* Uruchomienie srodowiska */
MPI_Init( &argc, &argv );
MPI_Comm_rank( MPI_COMM_WORLD, &rank );
MPI_Comm_size( MPI_COMM_WORLD, &size );

printf("Jestem procesem %d z %d\n", rank+1, size );
MPI_Finalize();
return 0;
}




