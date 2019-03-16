#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h> 
#include "types.h"

#define TIMESTAMP_LENGTH 10
#define BUFFER_INITIAL_CAPACITY 100

enum eResult
{
	SUCCESS,
	FAILED
};

static void sysLog(const char* msg, const enum eResult result);

static char* format(const struct event_t* event);

static struct buffer_t* currentBuffer = NULL;

void saveEvent(struct event_t* event);
struct event_t* createEvent(const long timestamp, const char* msg);
void flush();
