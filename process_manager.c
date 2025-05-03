/*
 *
 * process_manager.c
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stddef.h>
#include <pthread.h>
#include "queue.h"
#include <semaphore.h>

#define NUM_THREADS 2
sem_t Jamess; /*Semaforo exteno, declarado en factory*/


//Thread function
void *PrintHello(void *threadid)
{
   	long tid;
   	tid = (long)threadid;
   	printf("Hello World! It's me, thread #%ld!\n", tid);
	printf("Thread #%ld ends\n", tid);
   	pthread_exit(0);
}

/*process_manager (int id, int belt_size, int items_to_produce )*/
int *process_manager(void *arg) {
	sem_wait(&Jamess);
	pthread_t threads[NUM_THREADS];
   	int rc;
   	long t;

	//using the definded struct/function in queue.c
	struct element * elemento = NULL;
	queue_empty();	

	printf("Hello! I am the process manager and I have %d minions.\n", NUM_THREADS);

	for(t=0;t<NUM_THREADS;t++){
     		printf("Creating thread %ld\n", t);
		
		//launching a thread
     		rc = pthread_create(&threads[t], NULL, PrintHello, (void *)t);
     		
		if (rc){
       			printf("ERROR; return code from pthread_create() is %d\n", rc);
       			exit(-1);
       		}
     	}

	int i = 0;
	for(i = 0; i < NUM_THREADS; i++)
	{	
		pthread_join(threads[i], NULL);
	}
	printf("Fin process manager\n");
	sem_post(&Jamess);
   	/* Last thing that main() should do */
   	return(0);
	
}
