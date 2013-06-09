#include <mpi.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <assert.h>

#define FILENAME_SIZE 20
#define LINE_SIZE 80

#define REQUEST 0
#define REPORT 1

typedef struct {
    int x;
    int y;
} Position;

int rank, size;
MPI_Comm comm;

int main( int argc, char *argv[] )
{
    MPI_Init( &argc, &argv );
    MPI_Comm_rank( MPI_COMM_WORLD, &rank );
    MPI_Comm_size( MPI_COMM_WORLD, &size );
    printf( "Hello world from Jankiel %d of %d\n", rank, size );

	srand(time(0)*rank);   
	
	Position *positions;   
    int process_count;

	FILE *f;
	if (argc != 2) {
		f = fopen("./siatka.txt", "r");
	} else {
		f = fopen(argv[1], "r");
	}

	if (!f) {
		MPI_Abort(MPI_COMM_WORLD, 10);
	}
	char line[LINE_SIZE];
	fgets(line, LINE_SIZE, f);
	sscanf(line, "%d", &process_count);		
	
	assert(process_count == size);

	positions = (Position*)malloc(sizeof(Position) * process_count);		
			
	for (int i=0; i<process_count; i++) {
		int x, y;
		fgets(line, LINE_SIZE, f);
		sscanf(line, "%d %d", &x, &y);
		positions[i].x = x;
		positions[i].y = y;
	}
	
	fclose(f);


    MPI_Finalize();

    return 0;
}
