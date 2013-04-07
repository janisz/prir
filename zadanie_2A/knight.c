#include "knight.h"


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
				talk(data.name);
				break;
			} else if (!want_to_talk) {
				lock_cup_and_plate_mutex(data);
				wait_for_meal_and_eat(&data);
				break;
			} else {
				pthread_cond_wait(&data.cond[0], &data.table_m[LEFT]);
			}
		} while (repeat);
		unlock_mutex_and_broadcast(data);
	}

	quit_party(data);
	pthread_exit(NULL);
}

void wait_for_meal_and_eat(ThreadData *_data)
{
	ThreadData data = (*(ThreadData*)_data);
	do {
		if (can_eat(data)) {
			eat(_data);
			break;
		} else {
			pthread_cond_wait(&data.cond[1], &data.cup_m[CUCUMBER]);
		}
	} while (1);
}

void set_name(ThreadData *data)
{
	if (data->id == KING) {
		strncpy(data->name, "KING", NAME_SIZE);
	} else {
		snprintf(data->name, NAME_SIZE, "Knight #%.3d", data->id);
	}
}

int can_eat(ThreadData data)
{
	return (*data.king != IS_TALKING);
}

int can_talk(ThreadData data)
{
	return (*data.king != IS_TALKING);
}

void quit_party(ThreadData data)
{
	static int n;
	n++;
	printf("# %s finish %d\n", data.name, n);
}

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

int take_wine(ThreadData data)
{
	pthread_mutex_lock(data.bowl_m);
	int n = 0;
	do {
		if (*(data.bowl_state)) {
			n = *(data.bowl_state);
			(*data.bowl_state) = --n;
			break;
		} else {
			pthread_cond_wait(&data.cond[2], data.bowl_m);
		}
	} while (1);
	pthread_cond_signal(&data.cond[2]);
	pthread_mutex_unlock(data.bowl_m);
	return n;
}

int take_cucumber(ThreadData data)
{
	pthread_mutex_lock(data.pitcher_m);
	int n = 0;
	do {
		if (*(data.pitcher_state)) {
			n = *(data.pitcher_state);
			(*data.pitcher_state) = --n;
			break;
		} else {
			pthread_cond_wait(&data.cond[3], data.pitcher_m);
		}
	} while (1);
	pthread_cond_signal(&data.cond[3]);
	pthread_mutex_unlock(data.pitcher_m);
	return n;
}

void take_food(ThreadData data)
{
	fprintf(stderr, "Wine: %d \t Cucumber: %d\n", take_wine(data),	take_cucumber(data));
}

void talk(const char name[])
{
	int t = TALKING_TIME;
	printf("%s will talk for %d ms\n", name, t);
	usleep(t * 1000);
}

void eat(ThreadData *data)
{
	take_food(*data);
	data->round++;
	const char *suffixes[]= {"th","st","nd","rd"};
	int t = EATING_TIME;
	int n = data->round%10 < 4 ? data->round%10 : 0;
	printf("%s will eat&drink for %d%s time\n", data->name, data->round, suffixes[n]);
	usleep(t * 1000);
}
