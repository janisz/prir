#include <stdlib.h>

#define KING 0
#define KNIGHT_NAME_SIZE 15
//Time in miliseconds
#define WAITER_INTERVAL 1000
#define MAX_TALKING_TIME 500
#define MIN_TALKING_TIME 100
#define TALKING_TIME ((rand() % (MAX_TALKING_TIME - MIN_TALKING_TIME)) + MIN_TALKING_TIME)

#define TRUE (!FALSE)
#define FALSE (0)

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(*a))
#define FOREACH(item, array) \
    for(int keep = 1, \
            count = 0,\
            size = ARRAY_SIZE(array); \
        keep && count != size; \
        keep = !keep, count++) \
      for(item = (array) + count; keep; keep = !keep)


typedef struct {
	int  id;
} ThreadData;
