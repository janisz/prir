#include "jlib.h"
#include "knight.h"
#include "waiter.h"

int parse_arguments(int argc, char **argv);

int main(int argc, char** argv)
{
	srand(time(0));
	int people_count = parse_arguments(argc, argv);
	int bowl_state = people_count / 2;
	int pitcher_state = people_count / 3;
	int king = ACTIVE;

	printf("Party started! There will be %d people\n", people_count);

	ThreadData thread_data[people_count];
	ThreadData waiter_thread_data;
	pthread_cond_t cond[people_count];
	FOREACH(pthread_cond_t *c, cond) {
		pthread_cond_init(c, NULL);
	}
	pthread_mutex_t king_mutex, bowl_mutex, pitcher_mutex;
	pthread_mutex_t table_mutex[people_count];
	pthread_mutex_t cup_mutex[people_count];
	pthread_t people[people_count];
	pthread_t waiter;

	FOREACH(pthread_mutex_t *t, table_mutex) {
		pthread_mutex_init(t, NULL);
	}
	FOREACH(pthread_mutex_t *t, cup_mutex) {
		pthread_mutex_init(t, NULL);
	}

	init_mutex(3, &king_mutex, &bowl_mutex, &pitcher_mutex);


	for (int i=0; i<people_count; i++) {
		set_thread_data(&thread_data[i], i, people_count, &bowl_state, &pitcher_state, &king, cond, cup_mutex, &bowl_mutex, &pitcher_mutex, table_mutex, &king_mutex);
		pthread_create(&people[i], NULL, Knight, (void*)&thread_data[i]);
	}

	set_thread_data(&waiter_thread_data, -1, people_count, &bowl_state, &pitcher_state, &king, cond, cup_mutex, &bowl_mutex, &pitcher_mutex, table_mutex, &king_mutex);
	pthread_create(&waiter, NULL, Waiter, (void*)&waiter_thread_data);

	FOREACH(pthread_t *t, people) {
		pthread_join(*t, NULL);
	}
	pthread_cancel(waiter);
	FOREACH(pthread_cond_t *c, cond) {
		pthread_cond_destroy(c);
	}
	destroy_mutex(4, &king_mutex, &bowl_mutex, &pitcher_mutex, &table_mutex);

	return 0;
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




