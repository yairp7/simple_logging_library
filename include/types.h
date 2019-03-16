#include <sys/types.h>

struct event_t 
{
	long timestamp;
	char* msg;
} event_t;

static struct buffer_t
{
	struct event_t** events;
	unsigned int size;
	unsigned capacity;
	unsigned int flushId;
} buffer_t;
