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
 
 
 struct parametros {
	 int id;
	 int belt_size;
	 int num_products;
 };
 
 #define NUM_THREADS 2
 sem_t Jamess; /*Semaforo exteno, declarado en factory*/
 
 pthread_mutex_t mutex;
 pthread_cond_t not_full, not_empty;
 
 int total_products; // Número total de productos a generar
 
 void *producer(void *arg) {
	 struct parametros *params = (struct parametros *)arg;
 
	 for (int i = 0; i < params->num_products; i++) {
		 struct element product;
		 product.num_edition = i;
		 product.id_belt = params->id;
		 if (i == params->num_products - 1) {
			 product.last = 1;
		 } else {
			 product.last = 0;
		 }
 
		 pthread_mutex_lock(&mutex);
		 while (queue_full()) {
			 pthread_cond_wait(&not_full, &mutex);
		 }
 
		 queue_put(&product);
		 if (product.last) {
			 printf("[Producer][Belt %d] Produced edition %d (last)\n", params->id, product.num_edition);
		 } else {
			 printf("[Producer][Belt %d] Produced edition %d\n", params->id, product.num_edition);
		 }
 
		 pthread_cond_signal(&not_empty);
		 pthread_mutex_unlock(&mutex);
 
		 usleep(100000); // Simula tiempo de producción
	 }
 
	 pthread_exit(NULL);
 }
 
 void *consumer(void *arg) {
	 struct parametros *params = (struct parametros *)arg;
	 int done = 0;
 
	 while (!done) {
		 pthread_mutex_lock(&mutex);
		 while (queue_empty()) {
			 pthread_cond_wait(&not_empty, &mutex);
		 }
 
		 struct element *product = queue_get();
		 if (product->last) {
			 printf("[Consumer][Belt %d] Consumed edition %d (last)\n", params->id, product->num_edition);
		 } else {
			 printf("[Consumer][Belt %d] Consumed edition %d\n", params->id, product->num_edition);
		 }
 
		 if (product->last) {
			 done = 1;
		 }
 
		 pthread_cond_signal(&not_full);
		 pthread_mutex_unlock(&mutex);
 
		 usleep(150000); // Simula tiempo de consumo
	 }
 
	 pthread_exit(NULL);
 }
 
 void *process_manager(void *arg) {
	struct parametros *params = (struct parametros *)arg;

	sem_wait(&Jamess); // Sección crítica para evitar colisiones de impresión

	printf("[OK][process_manager] Process_manager with id %d waiting to produce %d elements.\n",params->id, params->num_products);

	sem_post(&Jamess); // Sección crítica para evitar colisiones de impresión

	 sem_wait(&Jamess); // Sección crítica para evitar colisiones de impresión
 
	 pthread_mutex_init(&mutex, NULL);
	 pthread_cond_init(&not_full, NULL);
	 pthread_cond_init(&not_empty, NULL);
 
	 
	 total_products = params->num_products;
 
	 printf("[ProcessManager %d] Starting with belt size %d and %d products\n",
			params->id, params->belt_size, params->num_products);

 
	 if (queue_init(params->belt_size) != 0) {
		 fprintf(stderr, "[ProcessManager %d] Error initializing queue\n", params->id);
		 sem_post(&Jamess);
		 pthread_exit(NULL);
	 }
	 printf("[OK][process_manager] Belt with id %d has been created with a maximum of %d elements.\n",params->id, params->belt_size);
 
	 pthread_t prod, cons;
	 pthread_create(&prod, NULL, producer, params);
	 pthread_create(&cons, NULL, consumer, params);
 
	 pthread_join(prod, NULL);
	 pthread_join(cons, NULL);
 
	 queue_destroy();

	 printf("[OK][process_manager] Process_manager with id %d has produced %d elements.\n", params->id, total_products);
	 printf("[OK][factory_manager] Process_manager with id %d has finished.\n", params->id);
 
	 sem_post(&Jamess);
 
	 pthread_mutex_destroy(&mutex);
	 pthread_cond_destroy(&not_full);
	 pthread_cond_destroy(&not_empty);
 
	 pthread_exit(NULL);
 }
 