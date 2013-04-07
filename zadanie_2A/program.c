#include "jlib.h"

int parse_arguments(int argc, char **argv);
int left(int position, int count);
int right(int position, int count);
void talk(const char name[]);
void eat(const ThreadData data);
void set_name(ThreadData *data);

void print_table_state(ThreadData d)
{
	for (int i=0; i<d.people_count; i++)
		printf("%d", d.table_state[i]);
	printf(" %s\n", d.name);
}

void set_table_state_and_broadcast(ThreadData data, int state)
{
//	pthread_mutex_lock(data.table_m);
	data.table_state[data.id] = state;
	pthread_cond_broadcast(&data.cond[0]);
	pthread_cond_broadcast(&data.cond[1]);
//	pthread_mutex_unlock(data.table_m);
}

int can_eat(ThreadData data)
{
	if (data.table_state[KING] == IS_TALKING) {
		return FALSE;
	}

	return TRUE;
}

int can_talk(ThreadData data)
{
	if (data.table_state[KING] == IS_TALKING) {
		return FALSE;
	}

	return TRUE;
}

int is_last(ThreadData data)
{
	return 1;
	int sum = 0;
	FOREACH(int *i, data.table_state) {
		sum += *i;
	}
	return sum == 1;
}

void quit_party(ThreadData data)
{
	static int n;
	set_table_state_and_broadcast(data, UNACTIVE);
	n++;
	print_table_state(data);
	printf("%s finish %d\n", data.name, n);
}

#define CUP  (((data.id%2) + data.id)%data.people_count)
#define CUCUMBER ((((data.id+1)%2) + data.id)%data.people_count)

#define LEFT  CUCUMBER // (MIN((data.id), ((data.id+1)%data.people_count)))
#define RIGHT CUP //(MAX((data.id), ((data.id+1)%data.people_count)))	

void lock_table_mutex(ThreadData data)
{

	pthread_mutex_lock(&data.table_m[RIGHT]);
	fprintf(stderr, "%s lock RIGHT\n", data.name);
	pthread_mutex_lock(&data.table_m[LEFT]);
	fprintf(stderr, "%s lock LEFT\n", data.name);

}

void unlock_table_mutex(ThreadData data)
{
	pthread_mutex_unlock(&data.table_m[LEFT]);
	pthread_mutex_unlock(&data.table_m[RIGHT]);
	fprintf(stderr, "%s unlocked table mutex\n", data.name);
}

void lock_cup_and_plate_mutex(ThreadData data)
{
	pthread_mutex_lock(&data.cup_m[CUP]);
	fprintf(stderr, "%s lock CUP\n", data.name);
	pthread_mutex_lock(&data.cup_m[CUCUMBER]);
	fprintf(stderr, "%s lock CUMCUMBER\n", data.name);
}

void unlock_cup_and_plate_mutex(ThreadData data)
{
	pthread_mutex_unlock(&data.cup_m[CUCUMBER]);
	pthread_mutex_unlock(&data.cup_m[CUP]);
	fprintf(stderr, "%s unlocked CUP adn PLATE\n", data.name);
}

void unlock_mutex_and_broadcast(ThreadData data)
{
	for (int i=0; i<data.people_count/2; i++)
		pthread_cond_signal(&data.cond[1]);
	unlock_cup_and_plate_mutex(data);
	for (int i=0; i<data.people_count/2; i++)
		pthread_cond_signal(&data.cond[0]);
	unlock_table_mutex(data);
}

void *Knight(void* thread_data)
{
	ThreadData data = (*(ThreadData*)thread_data);
	set_name(&data);
	printf("%s come\n", data.name);

	data.round = 0;

	while (data.round < MAX_ROUND) {
		int want_to_talk = RANDOM_BOOL;
		int repeat = 1;

		lock_table_mutex(data);

		do {

			if (want_to_talk && (can_talk(data))) {
				data.table_state[data.id] = IS_TALKING;
				talk(data.name);
				data.table_state[data.id] = ACTIVE;
				break;
			} else if (!want_to_talk) {

				lock_cup_and_plate_mutex(data);

				do {
					if (can_eat(data)) {
						data.table_state[data.id] = IS_EATING;
						data.round++;
						eat(data);
						data.table_state[data.id] = ACTIVE;
						repeat = 0;
						break;
					} else {
						pthread_cond_wait(&data.cond[1], &data.cup_m[CUCUMBER]);
					}
				} while (1);

				if (!repeat) break;
			} else {
				pthread_cond_wait(&data.cond[0], &data.table_m[LEFT]);
			}
		} while (repeat);
		unlock_mutex_and_broadcast(data);
	}

	quit_party(data);
	pthread_exit(NULL);
}

int main(int argc, char** argv)
{
	srand(time(0));
	int people_count = parse_arguments(argc, argv);
	int bowl_state = people_count / 2;
	int pitcher_state = people_count / 3;
	int table_state[people_count];

	for (int i=0; i<people_count; i++)
		table_state[i] = ACTIVE;

	printf("Party started! There will be %d people\n", people_count);

	ThreadData thread_data[people_count];
	pthread_cond_t cond[people_count];
	FOREACH(pthread_cond_t *c, cond) {
		pthread_cond_init(c, NULL);
	}
	pthread_mutex_t king_mutex, bowl_mutex, pitcher_mutex;
	pthread_mutex_t table_mutex[people_count];
	pthread_mutex_t cucumber_mutex[people_count / 2 +1];
	pthread_mutex_t cup_mutex[people_count];
	pthread_t people[people_count];

	FOREACH(pthread_mutex_t *t, table_mutex) {
		pthread_mutex_init(t, NULL);
	}
	FOREACH(pthread_mutex_t *t, cucumber_mutex) {
		pthread_mutex_init(t, NULL);
	}
	FOREACH(pthread_mutex_t *t, cup_mutex) {
		pthread_mutex_init(t, NULL);
	}

	init_mutex(4, &king_mutex, &bowl_mutex, &pitcher_mutex, &table_mutex);


	for (int i=0; i<people_count; i++) {
		thread_data[i].id = i;
		thread_data[i].cond = cond;
		thread_data[i].people_count = people_count;
		thread_data[i].cucumber_m = cucumber_mutex;
		thread_data[i].cup_m = cup_mutex;
		thread_data[i].table_m = table_mutex;
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
	FOREACH(pthread_cond_t *c, cond) {
		pthread_cond_destroy(c);
	}


	destroy_mutex(4, &king_mutex, &bowl_mutex, &pitcher_mutex, &table_mutex);

	return 0;
}

void set_name(ThreadData *data)
{
	if (data->id == KING) {
		strncpy(data->name, "KING", NAME_SIZE);
	} else {
		snprintf(data->name, NAME_SIZE, "Knight #%.3d", data->id);
	}
}

int left(int position, int count)
{
	return (position + count - 1) % count;
}

int right(int position, int count)
{
	return (position + 1) % count;
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

void eat(const ThreadData data)
{
	const char *suffixes[]= {"th","st","nd","rd"};
	int t = EATING_TIME;
	int n = data.round%10 < 4 ? data.round%10 : 0;
	printf("%s will eat&drink for %d%s time\n", data.name, data.round, suffixes[n]);
	usleep(t * 1000);
}


