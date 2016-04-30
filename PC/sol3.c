
/*
    Author: Chris Meyer
    Assignment: Lab 2 Solution 1
    Date: 4/28/16

    https://en.wikipedia.org/wiki/Producer%E2%80%93consumer_problem
*/
#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <semaphore.h>

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
sem_t fillCount;
sem_t emptyCount;
sem_t mutex;

// Pointers for producer and consumer index places
int removeIt = 0;
int addIt = 0;

static void putItemIntoBuffer(int item)
{

	printf("Added to buffer: %d \n", item);
	added++;
    sem_wait(&mutex);
    buffer[addIt] = item;
	addIt = (addIt + 1) % BUFFER_SIZE;

	sem_post(&mutex);
}

static int removeItemFromBuffer()
{
    //sem_wait(&mutex);
	int item = buffer[removeIt];
	printf("\t\t\tRemoved from buffer: %d \n", item);
	removed++;
	buffer[removeIt] = 0;
	removeIt = (removeIt + 1) % BUFFER_SIZE;
	//sem_post(&mutex);
	return (int) item;
}

/**
 * Thread job for the producers
 * Has NO SYNCHRONIZATION
 */
static void * producer(void* ptr){

    while(proLooped < LOOPX)
    {
        //sem_wait(&mutex);
        sem_wait(&emptyCount);

        putItemIntoBuffer(rand() % 100);
        proLooped++;


        sem_post(&fillCount);
        //sem_post(&mutex);
    }

	return (void*) ptr;
}

/**
 * Thread job for the consumers
 * Has NO SYNCHRONIZATION
 */
static void * consumer(void* ptr) {

    while(conLooped < LOOPX)
    {
        sem_wait(&fillCount);
        removeItemFromBuffer();
        conLooped++;
        sem_post(&emptyCount);
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

    // Init the semaphores fill is 0 and empty is equal to the buffer size
    sem_init(&fillCount, 0, 0);
    sem_init(&emptyCount, 0, BUFFER_SIZE);
    sem_init(&mutex, 0, 1); // Just needs 0 -> 1 because it's a lock

	// Makin threads
	// Producers
	pthread_t consumer1, producer1, consumer2,consumer3, producer2, producer3, producer4;
	pthread_create( &producer1, NULL, producer, NULL);
	pthread_create( &producer2, NULL, producer, NULL);
	pthread_create( &producer3, NULL, producer, NULL);
	pthread_create( &producer4, NULL, producer, NULL);

	// Consumers
	pthread_create( &consumer1, NULL, consumer, NULL);
	pthread_create( &consumer2, NULL, consumer, NULL);
	pthread_create( &consumer3, NULL, consumer, NULL);

	// Waiting for stuff
	pthread_join(consumer1, NULL);
	pthread_join(consumer2, NULL);
	pthread_join(consumer3, NULL);
	pthread_join(producer1, NULL);
	pthread_join(producer2, NULL);
	pthread_join(producer3, NULL);
	pthread_join(producer4, NULL);

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
