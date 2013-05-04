#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define FILENAME_SIZE 20
#define LINE_SIZE 80

int main( int argc, char *argv[] )
{
    int rank, size;

    MPI_Init( &argc, &argv );
    MPI_Comm_rank( MPI_COMM_WORLD, &rank );
    MPI_Comm_size( MPI_COMM_WORLD, &size );
    printf( "Hello world from process %d of %d\n", rank, size );
    if (rank == 0) {

        if (argc != 2) {
            printf("Specify graph file");
            exit(EXIT_FAILURE);
        }

        int vertices_count;

        FILE *f = fopen(argv[1], "r");
        char line[LINE_SIZE];
        fgets(line, LINE_SIZE, f);
        sscanf(line, "%d", &vertices_count);
        int edges_count = vertices_count -1;

        printf("%d %d\n", vertices_count, edges_count);

        int index[vertices_count];
        int edges[edges_count];
        int matrix_graph[vertices_count * vertices_count];
        for (int i=0; i<vertices_count*vertices_count; i++)
            matrix_graph[i] = 0;
        for (int i=0; i<edges_count; i++) {
            int from, to;
            fgets(line, LINE_SIZE, f);
            sscanf(line, "%d %d", &from, &to);
            matrix_graph[to*vertices_count+from] = 1;
        }

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

    MPI_Barrier(MPI_COMM_WORLD);

    MPI_Finalize();

    return 0;
}
