#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include "queue.h"

struct element* cola = NULL;

int tamano = 0; /*Tamano de la cola*/
int principio = 0; /*Posicion del primer elemento*/	
int final = 0; /*Posicion para insertar el siguiente elemento*/
int cont = 0; /*Numero de elementos en la cola*/

//To create a queue
int queue_init(int size){
	if (size <= 0) {
		return -1; /*Manejo de errores*/
	}
    cola = (struct element*)malloc(sizeof(struct element) * size); /*Se reserva el espacio segun los n elmentos*/
    if (!cola) {
		return -1;
	}

    tamano = size;
    principio = 0;
    final = 0;
    cont = 0;
    return 0;
}


// To Enqueue an element
int queue_put(struct element* x) {
	if (queue_full()) {
		return -1;
	} /*Manejo de errores*/
	
	cola[final] = *x; /*Se inserta el elemento en la cola*/
	final = (final + 1) % tamano; /*Se actualiza la posicion del final haciendo mod*/
	cont++; /*Se aumenta el contador*/
	return 0;
}


// To Dequeue an element.
struct element* queue_get(void) {
	if (queue_empty()) {
		return NULL; /*Manejo de errores*/
	}

    struct element* item = &cola[principio];
    principio = (principio + 1) % tamano;
    cont--;
    return item;
}


//To check queue state
int queue_empty(void){
	return (cont == 0);
}

int queue_full(void){
	return (cont == tamano);
}

//To destroy the queue and free the resources
int queue_destroy(void){
	if (cola) {
		free(cola);
	}
    cola = NULL;
    tamano = 0;
    principio = 0;
    final = 0;
    cont = 0;
    return 0;
}
