#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include "queue.h"

char *cola;
//To create a queue
int queue_init(int size){
	cola = (char *) malloc(size);
	if (!cola){
		return -1;
	}
	
	return 0;
}


// To Enqueue an element
int queue_put(struct element* x) {

	return 0;
}


// To Dequeue an element.
struct element* queue_get(void) {
	return NULL;
}


//To check queue state
int queue_empty(void){
	return 0;
}

int queue_full(void){
	return 0;
}

//To destroy the queue and free the resources
int queue_destroy(void){
	free(cola);
	return 0;
}
