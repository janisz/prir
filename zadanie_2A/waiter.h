#ifndef WAITER_H_
#define WAITER_H_

#include "jlib.h"

void *Waiter(void* thread_data);

void add_food(ThreadData data);
void add_cucumbers(ThreadData data);
void wine_refill(ThreadData data);

#endif


