
#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <time.h>

// Teh globals.
// such scope, many access, very mutable, wow.
int BUFFER_SIZE = 25;
int itemCount;
int LOOPX;
int *buffer;
int added = 0;
int removed = 0;
int proLooped = 0;
int conLooped = 0;

// Pointers for producer and consumer index places
int removeIt = 0;
int addIt = 0;

static void putItemIntoBuffer(int item)
{
	printf("Added to buffer: %d \n", item);
	added++;
    	buffer[addIt] = item;
	addIt = (addIt + 1) % BUFFER_SIZE;
}

static int removeItemFromBuffer()
{
	int item = buffer[removeIt];
	printf("\t\t\tRemoved from buffer: %d \n", item);
	removed++;
	buffer[removeIt] = 0;
	removeIt = (removeIt + 1) % BUFFER_SIZE;
	return (int) item;
}

/**
 * Thread job for the producers
 * Has NO SYNCHRONIZATION
 */
static void * producer(void* ptr){
	while (proLooped < LOOPX)
	{
		if (itemCount < BUFFER_SIZE)
		{
			putItemIntoBuffer(rand() % 100);
			itemCount = itemCount + 1;
			proLooped++;
		}
	}

	return (void*) ptr;
}

/**
 * Thread job for the consumers
 * Has NO SYNCHRONIZATION
 */
static void * consumer(void* ptr) {
	while (conLooped < LOOPX)
	{
		if (itemCount > 0)
		{
			removeItemFromBuffer();
			itemCount = itemCount - 1;
			conLooped++;
		}
	}

	return (void*) ptr;
}

/**
 * Main entry point to the program
 */
int main(int argc, char **argv) {

	LOOPX = 100;   // Number of times each thing will run.
	itemCount = 0;
	buffer = malloc(sizeof(int) * BUFFER_SIZE);

	srand(time(NULL)); // See our random number generator

	// Makin threads
	pthread_t consumerT, producerT;
	pthread_create( &producerT, NULL, producer, NULL);
	pthread_create( &consumerT, NULL, consumer, NULL);

	// Waiting for stuff
	pthread_join(consumerT, NULL);
	pthread_join(producerT, NULL);

	// Everything finished, so blat out the buffer
	int i;
	printf("In buffer:\n");
	for(i = 0; i < BUFFER_SIZE; i++) {
		printf("%d\n", buffer[i]);
	}

	printf("Loops:  Producer = %d, Consumer = %d\n", proLooped, conLooped);
	printf("Added: %d  Removed: %d\n", added, removed);

	return 1;
}
