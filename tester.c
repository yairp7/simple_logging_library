#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dlfcn.h>
#include <time.h>
#include <unistd.h>
#include "include/types.h"

#define NUM_EVENTS 10

int main(int argc, char* argv[]) 
{
	void* handle = dlopen("bin/liblogging.so", RTLD_NOW|RTLD_GLOBAL);
	if(handle)
	{
		struct event_t* (*addEvent)(const long, const char*);
		void (*flush)();
		addEvent = (struct event_t* (*)(const long, const char*))dlsym(handle, "createEvent");
		flush = (void (*)())dlsym(handle, "flush");
		if(addEvent && flush)
		{
			char* msg = alloca(64);
			for(int x = 0; x < 1; x++)
			{
				for(int i = 0; i < NUM_EVENTS; i++)
				{
					memset(msg, 0, 64);
					sprintf(msg, "%d|%d: %s", getpid(), (x * 10) + i, "booboob");
					struct event_t* event = addEvent(time(0), msg);
				}
				flush();
			}
		}
		else
		{
			printf("Failed getting library symbol!\n");
		}
		dlclose(handle);
	}
	else
	{
		printf("Failed opening library!\n");
	}
	return 0;
}