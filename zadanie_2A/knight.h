#ifndef KNIGHT_H_
#define KNIGHT_H_

#include "jlib.h"

void *Knight(void* thread_data);

void set_name(ThreadData *data);

void wait_for_meal_and_eat(ThreadData *_data);
int can_eat(ThreadData data);
void take_food(ThreadData data);
void eat(ThreadData *data);
int take_wine(ThreadData data);
int take_cucumber(ThreadData data);

int can_talk(ThreadData data);
void talk(const char name[]);

void set_king_state(ThreadData data, int new_value);
int get_king_state(ThreadData data); 

void quit_party(ThreadData data);

void lock_table_mutex(ThreadData data);
void unlock_table_mutex(ThreadData data);

void lock_cup_and_plate_mutex(ThreadData data);
void unlock_cup_and_plate_mutex(ThreadData data);

void unlock_mutex_and_broadcast(ThreadData data);

#endif
