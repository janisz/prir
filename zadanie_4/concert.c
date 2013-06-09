#include <mpi.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <assert.h>

#define ABS(a) (((a) < 0) ? -(a) : (a))

#define FILENAME_SIZE 20
#define LINE_SIZE 80

#define MAX_DISTANCE 9

#define FALSE 0
#define TRUE (!FALSE)

typedef struct {
    int x;
    int y;
} Position;

Position *positions;
int process_count;
int rank, size;
MPI_Comm comm;
int has_played = FALSE;
int will_play_in_this_turn = FALSE;

int distance(Position a, Position b)
{
    return (b.x-a.x)*(b.x-a.x) + (b.y-a.y)*(b.y-a.y);
}

int can_play(int id)
{
    if (distance(positions[rank], positions[id]) > MAX_DISTANCE) {
//        printf("%d:\t %d is too far\n", rank, id);
        return TRUE;
    }
    if (id < rank)
        return TRUE;
    return !will_play_in_this_turn;
}

void read_data_from_file(int argc, char *argv[])
{
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

    if (rank == 0)
        for (int i=0; i<process_count; i++) {
            printf("%d %d\n", positions[i].x, positions[i].y);
        }

    fclose(f);
    printf( "%d data loaded\n", rank);
}

int main( int argc, char *argv[] )
{
    MPI_Init( &argc, &argv );
    MPI_Comm_rank( MPI_COMM_WORLD, &rank );
    MPI_Comm_size( MPI_COMM_WORLD, &size );
    printf( "Hello world from Jankiel %d of %d\n", rank, size );

    read_data_from_file(argc, argv);
    MPI_Barrier(MPI_COMM_WORLD);

    int sleeping_count = 0;
    int turn = 0;
    while (sleeping_count != process_count) {
        turn++;
        MPI_Barrier(MPI_COMM_WORLD);
        if (rank == 0) printf("####\t%d\t####\n", turn);
        
        int token = 0;
        will_play_in_this_turn = FALSE;
        for (token = 0; token<process_count; token++) {
			MPI_Barrier(MPI_COMM_WORLD);
            int can_token_owner_play = TRUE;
            int token_owner_result = 0;
            int token_owner_has_played = has_played;

            MPI_Bcast(&token_owner_has_played, 1, MPI_INT, token, MPI_COMM_WORLD);
            if (!token_owner_has_played && rank != token) {
                can_token_owner_play = can_play(token);
            }
            MPI_Reduce( &can_token_owner_play, &token_owner_result, 1, MPI_INT, MPI_SUM, token, MPI_COMM_WORLD );

            will_play_in_this_turn |= (rank == token) && (!has_played) && (token_owner_result == process_count);            
        }

        if (will_play_in_this_turn)
            printf("%d:\tWILL PLAY\n", rank);

        has_played |= will_play_in_this_turn;
        MPI_Allreduce( &has_played, &sleeping_count, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD );
    }
    MPI_Finalize();

    return 0;
}
