#include "jlib.h"

void init_mutex(int size,  ...)
{
	va_list ap;
	va_start(ap, size);
	for (; size > 0; size--) {
		pthread_mutex_t *m = va_arg(ap, pthread_mutex_t*);
		pthread_mutex_init(m, NULL);
	}

	va_end(ap);
}

void destroy_mutex(int size,  ...)
{
	va_list ap;
	va_start(ap, size);
	for (; size > 0; size--) {
		pthread_mutex_t *m = va_arg(ap, pthread_mutex_t*);
		pthread_mutex_destroy(m);
	}

	va_end(ap);
}

void set_thread_data(ThreadData *data, int id, int people_count, int *bowl_state,
                     int *pitcher_state, int *king, pthread_cond_t *cond,
						pthread_mutex_t *cup_m, pthread_mutex_t *bowl_m,
                     pthread_mutex_t *pitcher_m, pthread_mutex_t *table_m, pthread_mutex_t *king_m)
{
	data->id = id;
	data->people_count = people_count;
	data->bowl_state = bowl_state;
	data->pitcher_state = pitcher_state;
	data->king = king;	
	data->cond = cond;
	data->cup_m = cup_m;
	data->bowl_m = bowl_m;
	data->pitcher_m = pitcher_m;
	data->table_m = table_m;
	data->king_m = king_m;
}

int random_number()
{
	unsigned seed = SEED;
	return rand_r(&seed);
}
