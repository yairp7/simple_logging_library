#include <time.h>
#include "include/logging.h"

void failedAllocating() {
	sysLog("Failed allocating.", FAILED);
	exit(1);
}

static void sysLog(const char* msg, const enum eResult result)
{
	if(result == SUCCESS) {
		printf("[+] %s\n", msg);
	}
	else if(result == FAILED) {
		printf("[-] %s\n", msg);
	}
	else {
		printf("%s\n", msg);
	}
}

static char* format(const struct event_t* event) 
{
	if(event)  {
		if(event->msg) {
			unsigned int len = strlen(event->msg) + TIMESTAMP_LENGTH + 2;
			char* output = (char*)calloc(len + 1, sizeof(char));
			if(output) {
				sprintf(output, "%ld|%s", event->timestamp, event->msg);
				return output;
			}
			else {
				failedAllocating();
			}
		}
	}
	return NULL;
}

void setOutputFile(char* filename)
{
	strcpy(outputFile, filename);
}

struct event_t* createEvent(const long timestamp, const char* msg)
{
	struct event_t* event = (struct event_t*)calloc(1, sizeof(struct event_t));
	if(event) {
		event->timestamp = timestamp;
		int len = strlen(msg);
		event->msg = (char*)calloc(len + 1, sizeof(char));
		if(event->msg) {
			memcpy(event->msg, msg, len);
			saveEvent(event);

			return event;
		}
		else {
			failedAllocating();
		}
	}
	return NULL;
}

void saveEvent(struct event_t* event) 
{
	if(currentBuffer == NULL) {
		currentBuffer = (struct buffer_t*)calloc(1, sizeof(struct buffer_t));
		if(currentBuffer) {
			currentBuffer->size = 0;
			currentBuffer->capacity = BUFFER_INITIAL_CAPACITY;
			currentBuffer->events = (struct event_t**)calloc(currentBuffer->capacity, sizeof(struct event_t*));
			if(currentBuffer) {
				currentBuffer->flushId = 0;
			}
			else {
				failedAllocating();
			}
		}
		else {
			failedAllocating();
		}
	}

	if(currentBuffer->size > currentBuffer->capacity * 0.75) {
		currentBuffer->capacity = currentBuffer->capacity * 2;
		unsigned int oldSize = currentBuffer->size;
		struct event_t** oldEvents = currentBuffer->events;
		struct event_t** newEvents = (struct event_t**)calloc(currentBuffer->capacity, sizeof(struct event_t*)); 
		if(newEvents) {
			for(unsigned int i = 0; i < oldSize; i++) {
				newEvents[i] = oldEvents[i];
			}
			currentBuffer->events = newEvents;
		}
		else {
			failedAllocating();
		}
		free(oldEvents);
	}

	currentBuffer->events[currentBuffer->size] = event;
	currentBuffer->size++;
}	

void flush()
{
	if(currentBuffer) {
		pid_t pid = fork();
		if(pid == 0) { // child
			char outputFileName[32];
			sprintf(outputFileName, "output/%s_%d.log", "output", currentBuffer->flushId);
			FILE* outputFile = fopen(outputFileName, "w+");
			if(outputFile) {
				for(unsigned int i = 0; i < currentBuffer->size; i++) {
					char* eventString = format(currentBuffer->events[i]);
					fprintf(outputFile, "%s\n", eventString);
					free(eventString);
				}
				fclose(outputFile);
				exit(0);
			}
			else {
				exit(1); 
			}
		}
		else {
			currentBuffer->flushId++;
			currentBuffer->events = (struct event_t**)realloc(currentBuffer->events, BUFFER_INITIAL_CAPACITY);
			currentBuffer->capacity = BUFFER_INITIAL_CAPACITY;
			unsigned int oldSize = currentBuffer->size;
			currentBuffer->size = 0;
			struct event_t** newEvents = (struct event_t**)calloc(sizeof(struct event_t*), BUFFER_INITIAL_CAPACITY);
			struct event_t** oldEvents = currentBuffer->events;
			currentBuffer->events = newEvents; 
			for(unsigned int i = 0; i < oldSize; i++) {
				free(oldEvents[i]->msg);
				free(oldEvents[i]);
			}
			free(oldEvents);
		}
	}	
}
