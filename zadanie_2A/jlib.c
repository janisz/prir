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

int random_number()
{
	unsigned seed = SEED;
	return rand_r(&seed);
}
