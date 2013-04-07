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

#define CUP  (((data.id%2) + data.id)%data.people_count)
#define CUCUMBER ((((data.id+1)%2) + data.id)%data.people_count)

#define LEFT  CUCUMBER // (MIN((data.id), ((data.id+1)%data.people_count)))
#define RIGHT CUP //(MAX((data.id), ((data.id+1)%data.people_count)))	

//Time in miliseconds
#define MAX_TALKING_TIME 500
#define MIN_TALKING_TIME 100
#define SEED (getpid() * time(NULL) * (pthread_self() * 31 + 1))
#define TALKING_TIME ((random_number() % (MAX_TALKING_TIME - MIN_TALKING_TIME)) + MIN_TALKING_TIME)
#define EATING_TIME TALKING_TIME

#define RANDOM_BOOL ((random_number() % 2))

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
	int *king;
	char name[NAME_SIZE];
	pthread_cond_t *cond;
	pthread_mutex_t *cup_m;
	pthread_mutex_t *bowl_m;
	pthread_mutex_t *pitcher_m;
	pthread_mutex_t *table_m;
	pthread_mutex_t *king_m;
} ThreadData;

void set_thread_data(ThreadData *data, int id, int people_count, int *bowl_state,
                     int *pitcher_state, int *king, pthread_cond_t *cond,
						pthread_mutex_t *cup_m, pthread_mutex_t *bowl_m,
                     pthread_mutex_t *pitcher_m, pthread_mutex_t *table_m, pthread_mutex_t *king_m);

void init_mutex(int size,  ...);
void destroy_mutex(int size,  ...);
int random_number();

#endif
