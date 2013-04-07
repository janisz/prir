#include "waiter.h"

void *Waiter(void* thread_data)
{
	ThreadData data = (*(ThreadData*)thread_data);
	strncpy(data.name, "> Waiter", NAME_SIZE);

	while(1) {
		add_food(data);
		usleep(1000 * TALKING_TIME * data.people_count / 3);
	}

	pthread_exit(NULL);
}

void wine_refill(ThreadData data)
{
	pthread_mutex_lock(data.bowl_m);
	*data.bowl_state = data.people_count/2;
	pthread_mutex_unlock(data.bowl_m);
	pthread_cond_signal(&data.cond[WINE_COND]);
}

void add_cucumbers(ThreadData data)
{
	pthread_mutex_lock(data.pitcher_m);
	*data.pitcher_state = data.people_count/3;
	pthread_mutex_unlock(data.pitcher_m);
	pthread_cond_signal(&data.cond[CUCUMBER_COND]);
}

void add_food(ThreadData data)
{
	add_cucumbers(data);
	wine_refill(data);
	printf("%s add food\n", data.name);
}
