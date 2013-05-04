#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define FILENAME_SIZE 20
#define LINE_SIZE 80

int main( int argc, char *argv[] )
{
	if (argc != 2) {
		printf("Specify graph file");
		exit(EXIT_FAILURE);
	}
		
	int vertexs_count;
	
	FILE *f = fopen(argv[1], "r");
	char line[LINE_SIZE];
	fgets(line, LINE_SIZE, f);
	sscanf(line, "%d", &vertexs_count); 
	int edges_count = vertexs_count -1;
	
	printf("%d %d\n", vertexs_count, edges_count);
	
	int index[vertexs_count];
	int edges[edges_count];
	int matrix_graph[vertexs_count * vertexs_count];
	for (int i=0; i<edges_count;i++) {
		int from, to;
		fgets(line, LINE_SIZE, f);
		sscanf(line, "%d %d", &from, &to);
		matrix_graph[to*vertexs_count+from] = 1;
	}
	
	for (int i=0; i<vertexs_count;i++) {
		for (int j=0; j<vertexs_count;j++) {
			matrix_graph[i*vertexs_count+j] = matrix_graph[i*vertexs_count+j] != 1 ? 0 : 1;
			printf("%d ", matrix_graph[i*vertexs_count+j]);
		}
		printf("\n");
	}
	
	return 0;
}
