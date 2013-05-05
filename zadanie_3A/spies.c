#include <mpi.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#define FILENAME_SIZE 20
#define LINE_SIZE 80

#define REQUEST 0
#define REPORT 1

typedef struct {
    int time;
    int count;
} Data;

int rank, size;
MPI_Comm comm;

int main( int argc, char *argv[] )
{
    MPI_Init( &argc, &argv );
    MPI_Comm_rank( MPI_COMM_WORLD, &rank );
    MPI_Comm_size( MPI_COMM_WORLD, &size );
    printf( "Hello world from process %d of %d\n", rank, size );

	srand(time(0)*rank);   
	
	int *index;
    int *edges;
    int edges_count;
    int vertices_count;

	if (!rank) {
		FILE *f;
		if (argc != 2) {
			f = fopen("./siatka", "r");
		} else {
			f = fopen(argv[1], "r");
		}

		

		
		if (!f) {
			MPI_Abort(MPI_COMM_WORLD, 10);
		}
		char line[LINE_SIZE];
		fgets(line, LINE_SIZE, f);
		sscanf(line, "%d", &vertices_count);
		edges_count = vertices_count -1;

		printf("%d %d\n", vertices_count, edges_count);

		index = (int*)malloc(sizeof(int) * vertices_count);
		edges = (int*)malloc(sizeof(int) * edges_count);

		int matrix_graph[vertices_count * vertices_count];
		for (int i=0; i<vertices_count*vertices_count; i++)
			matrix_graph[i] = 0;
		for (int i=0; i<edges_count; i++) {
			int from, to;
			fgets(line, LINE_SIZE, f);
			sscanf(line, "%d %d", &from, &to);
			matrix_graph[to*vertices_count+from] = 1;
		}
		
		fclose(f);

		for (int i=0; i<vertices_count; i++) {
			for (int j=0; j<vertices_count; j++) {
				matrix_graph[i*vertices_count+j] = matrix_graph[i*vertices_count+j] != 1 ? 0 : 1;
				printf("%d ", matrix_graph[i*vertices_count+j]);
			}
			printf("\n");
		}

		int edge_index = 0;
		for (int i=0; i<vertices_count; i++) {
			int neighbours = 0;
			for (int j=0; j<vertices_count; j++) {
				if (matrix_graph[j*vertices_count+i] == 1) {
					neighbours++;
					edges[edge_index] = j;
					edge_index++;
				}
			}
			index[i] = edge_index;
		}

		printf("Node \t index \t edges\n");
		edge_index = 0;
		for (int i=0; i<vertices_count; i++) {
			printf("%d \t %d \t", i, index[i]);
			int current_index = index[i];
			for (int i = edge_index; i < current_index; i++)
				printf("%d ", edges[i]);
			printf("\n");
			edge_index = index[i];

		}
		
	}
	
	MPI_Bcast(&vertices_count, 1, MPI_INT, 0, MPI_COMM_WORLD);
	edges_count = vertices_count - 1;
	if (rank) {
		index = (int*)malloc(sizeof(int) * vertices_count);
		edges = (int*)malloc(sizeof(int) * edges_count);
	}

	MPI_Bcast(index, vertices_count, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(edges, edges_count, MPI_INT, 0, MPI_COMM_WORLD);    
	MPI_Graph_create( MPI_COMM_WORLD, vertices_count, index, edges, 0, &comm );
	
	int neighbors_count = 0;
	MPI_Graph_neighbors_count(comm, rank, &neighbors_count);
	printf("#%d has %d neighbors\n", rank, neighbors_count); 
 
	 
	if (!neighbors_count) {
	
		Data raport;
		MPI_Status status;
		MPI_Request request;
				
		MPI_Irecv(&raport, 1, MPI_2INT, MPI_ANY_SOURCE, REQUEST, comm, &request);
		int sleep_time = rand()%5+3;
		printf("#%d spying for %d s\n", rank, sleep_time);
		
		sleep(sleep_time);				
		
		MPI_Wait(&request, &status);
		
		raport.time = time(0);//rand()%100;
		raport.count = rand()%100;
		
		MPI_Isend(&raport, 1, MPI_2INT, status.MPI_SOURCE, REPORT, comm, &request);
		printf("#%d sent raport to #%d:  %d ; %d\n", rank, status.MPI_SOURCE, raport.time, raport.count);
		MPI_Wait(&request, &status);
		printf("#%d finished his job", rank);		
	}
	else {
		Data raport;
		Data reports[neighbors_count];
		int head;
		int neighbors[neighbors_count];
		MPI_Status statuses[neighbors_count];
		MPI_Request requests[neighbors_count];					
		
		MPI_Request request;
		MPI_Status status;
		
		MPI_Graph_neighbors(comm, rank, neighbors_count, neighbors);
		
		if (rank != 0) {
			MPI_Irecv(&head, 1, MPI_INT, MPI_ANY_SOURCE, REQUEST, comm, &request);
			MPI_Wait(&request, &status);
		}
		
		for (int i = 0; i < neighbors_count; i++) {
			MPI_Isend(&rank, 1, MPI_INT, neighbors[i], REQUEST, comm, &requests[i]);
			printf("#%d sent raport request to #%d\n", rank, neighbors[i]);
		}
		
		sleep(rand()%6+2);
		
		printf("#%d start reports gathering\n", rank);
		
		for (int i = 0; i < neighbors_count; i++) {
			MPI_Irecv(&reports[i], 1, MPI_2INT, neighbors[i], REPORT, comm, &requests[i]);
		}
		
		MPI_Waitall(neighbors_count, requests, statuses);
		
		raport.time = -1;
		raport.count = -1;
		
		for (int i = 0; i < neighbors_count; i++) {
			if (raport.time < reports[i].time || 
			(raport.time == reports[i].time && raport.count < reports[i].count)) {
				raport.time = reports[i].time;
				raport.count = reports[i].count;
			}
		}
		
		printf("#%d reports:\t", rank);
		for (int i = 0; i < neighbors_count; i++) {
			printf("%d %d, ", reports[i].time, reports[i].count);
		}
		printf("\n");
		
		if (rank) {
			printf("#%d sent raport to #%d:  %d ; %d\n", rank, status.MPI_SOURCE, raport.time, raport.count);
			MPI_Isend(&raport, 1, MPI_2INT, status.MPI_SOURCE, REPORT, comm, &request);
			MPI_Wait(&request, &status);
		}
		else {
			printf("Final raport:  %d ; %d\n", raport.time, raport.count);			
		}
	}
 
    MPI_Finalize();

    return 0;
}
