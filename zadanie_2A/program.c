#define _GNU_SOURCE
#include <pthread.h>
#include <stdio.h>
#include <time.h>
#include "jlib.h"

int people_count = 0;
int parse_arguments(int argc, char **argv);
void talk(const char name[]);
void *King()
{
	printf("KING come\n");

	printf("KING finish\n");
	pthread_exit(NULL);
}

void *Knight(void* thread_data)
{
	ThreadData data = (*(ThreadData*)thread_data);
	char name[KNIGHT_NAME_SIZE];
	snprintf(name, KNIGHT_NAME_SIZE, "Knight #%.3d", data.id);
	printf("%s come\n", name);

	talk(name);

	printf("%s finish\n", name);
	pthread_exit(NULL);
}

int main(int argc, char** argv)
{
	srand(time(0));
	people_count = parse_arguments(argc, argv);
	printf("Party started! There will be %d people\n", people_count);
	pthread_t people[people_count];
	ThreadData thread_data[people_count];

	pthread_create(people, NULL, King, NULL);
	for (int i=1; i<=people_count; i++) {
		thread_data[i].id = i;
		pthread_create(&people[i], NULL, Knight, (void*)&thread_data[i]);
	}

	FOREACH(pthread_t *t, people)
	pthread_join(*t, NULL);

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

void talk(const char name[])
{
	int t = TALKING_TIME;
	printf("%s will talk for %d ms\n", name, t);
	usleep(t * 1000);
}
