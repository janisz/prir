
#include "jlib.h"

int parse_arguments(int argc, char **argv);
int left(int position, int count);
int right(int position, int count);
void talk(const char name[]);
void set_name(char *name, ThreadData data);

void *Knight(void* thread_data)
{
	ThreadData data = (*(ThreadData*)thread_data);
	char name[NAME_SIZE];
	set_name(name, data);

	int round = 0;
	while (round < MAX_ROUND) {
		int want_to_talk = RANDOM_BOOL;

		if (want_to_talk) {
			pthread_mutex_lock(data.table_m);
			do {
				if (RANDOM_BOOL) {
					talk(name);										
					pthread_cond_broadcast(data.cond);	
					break;
				} else
					pthread_cond_wait(data.cond, data.table_m);
			} while (1);					
			pthread_mutex_unlock(data.table_m);
		} else {
			printf("%s eat\n", name);
			round++;
		}


	}

	printf("%s finish\n", name);
	pthread_exit(NULL);
}

int main(int argc, char** argv)
{
	srand(time(0));
	int people_count = parse_arguments(argc, argv);
	int bowl_state = people_count / 2;
	int pitcher_state = people_count / 3;
	int table_state[people_count];
	printf("Party started! There will be %d people\n", people_count);

	ThreadData thread_data[people_count];
	pthread_cond_t  cond = PTHREAD_COND_INITIALIZER;
	pthread_mutex_t king_mutex, bowl_mutex, pitcher_mutex, table_mutex;
	pthread_t people[people_count];

	pthread_cond_init(&cond, NULL);
	init_mutex(4, &king_mutex, &bowl_mutex, &pitcher_mutex, &table_mutex);

	for (int i=0; i<people_count; i++) {
		table_state[i] = FALSE;
		thread_data[i].id = i;
		thread_data[i].cond = &cond;
		thread_data[i].people_count = people_count;
		thread_data[i].king_m = &king_mutex;
		thread_data[i].table_m = &table_mutex;
		thread_data[i].bowl_m = &bowl_mutex;
		thread_data[i].pitcher_m = &pitcher_mutex;
		thread_data[i].bowl_state = &bowl_state;
		thread_data[i].pitcher_state = &pitcher_state;
		thread_data[i].table_state = table_state;
		pthread_create(&people[i], NULL, Knight, (void*)&thread_data[i]);
	}

	FOREACH(pthread_t *t, people) {
		pthread_join(*t, NULL);
	}

	pthread_cond_destroy(&cond);
	destroy_mutex(4, &king_mutex, &bowl_mutex, &pitcher_mutex, &table_mutex);

	return 0;
}

void set_name(char *name, ThreadData data)
{
	if (data.id == KING) {
		strncpy(name, "KING", NAME_SIZE);
	} else {
		snprintf(name, NAME_SIZE, "Knight #%.3d", data.id);
		printf("%s come\n", name);
	}
}

int left(int position, int count)
{
	return (position + count - 1) % count;
}

int right(int position, int count)
{
	return (position + count + 1) % count;
}

int parse_arguments(int argc, char **argv)
{
	if (argc != 2)
		exit(EXIT_FAILURE);
	int number = atoi(argv[1]) + 1;
	if (number < 5 || number % 2 == 1)
		exit(EXIT_FAILURE);
	return number;
}

void talk(const char name[])
{
	int t = TALKING_TIME;
	printf("%s will talk for %d ms\n", name, t);
	usleep(t * 1000);
}
