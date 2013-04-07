#ifndef JLIB_H_
#define JLIB_H_

#define _GNU_SOURCE
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>
#include <unistd.h>

#define KING 0
#define NAME_SIZE 15
#define NEIGHBORS_COUNT 2
#define MAX_ROUND 10

#define UNACTIVE 0
#define ACTIVE 1
#define IS_TALKING 3
#define IS_EATING 5

#define MIN(a, b)  (((a) < (b)) ? (a) : (b))
#define MAX(a, b)  (((a) > (b)) ? (a) : (b))

//Time in miliseconds
#define WAITER_INTERVAL 1000
#define MAX_TALKING_TIME 500
#define MIN_TALKING_TIME 100
#define TALKING_TIME ((rand() % (MAX_TALKING_TIME - MIN_TALKING_TIME)) + MIN_TALKING_TIME)
#define EATING_TIME TALKING_TIME

#define RANDOM_BOOL ((rand() % 2))

#define FALSE (0)
#define TRUE (!FALSE)

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(*a))
#define FOREACH(item, array) \
    for(int keep = 1, \
            count = 0,\
            size = ARRAY_SIZE(array); \
        keep && count != size; \
        keep = !keep, count++) \
      for(item = (array) + count; keep; keep = !keep)


typedef struct {
	int id;
	int people_count;
	int round;
	int *bowl_state;
	int *pitcher_state;
	int *table_state;
	char name[NAME_SIZE];
	pthread_cond_t *cond;
	pthread_mutex_t *cucumber_m;
	pthread_mutex_t *cup_m;
	pthread_mutex_t *bowl_m;
	pthread_mutex_t *pitcher_m;
	pthread_mutex_t *table_m;
	pthread_mutex_t *king_m;
} ThreadData;

void init_mutex(int size,  ...);
void destroy_mutex(int size,  ...);

#endif
