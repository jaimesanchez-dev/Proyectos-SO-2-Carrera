#include <stdio.h>

#include <stdlib.h>

#include <pthread.h>

#include <semaphore.h>

#include "process_manager.h"

#include "queue.h"



pthread_mutex_t mutex;

pthread_cond_t not_full, not_empty;



int produced_count = 0;

int consumed_count = 0;



void *producer(void *arg) {

    struct parametros *params = (struct parametros *)arg;

    int produced = 0;

    


    

    while (produced < params->num_products) {

        pthread_mutex_lock(&mutex);

        

        while (produced_count != 0) { // espera a que se haya consumido el lote anterior

            pthread_cond_wait(&not_full, &mutex);

        }

        

        int lote = 0;

        

        while (lote < params->belt_size && produced < params->num_products) {

            struct element product;

            product.num_edition = produced;

            product.id_belt = params->id;

            product.last = (produced == params->num_products - 1) ? 1 : 0;

            

            queue_put(&product);

            

            printf("[OK][queue] Introduced element with id %d in belt %d.\n", product.num_edition, params->id);

            

            lote++;

            produced++;

            produced_count++;

        }

        

        pthread_cond_signal(&not_empty); // avisa al consumidor que puede empezar

        pthread_mutex_unlock(&mutex);

    }

    

    pthread_exit(NULL);

}



void *consumer(void *arg) {

    struct parametros *params = (struct parametros *)arg;

    int done = 0;

    

    while (!done) {

        pthread_mutex_lock(&mutex);

        

        while (produced_count == 0) { // espera hasta que haya productos

            pthread_cond_wait(&not_empty, &mutex);

        }

        

        while (produced_count > 0) {

            struct element *product = queue_get();

            printf("[OK][queue] Obtained element with id %d in belt %d.\n", product->num_edition, params->id);

            

            if (product->last) {

                done = 1;

            }

            

            produced_count--;

            consumed_count++;

        }

        

        consumed_count = 0;

        pthread_cond_signal(&not_full); // avisa al productor que puede hacer otro lote

        pthread_mutex_unlock(&mutex);

    }

    

    pthread_exit(NULL);

}



void *process_manager(void *arg) {

    struct parametros *p = arg;

    

    // 1) Esperar la señal inicial del factory_manager

    sem_wait(&p->start_sem[p->index]);

    

    // 2) Imprimir el mensaje de espera e inicializar estructuras

    printf("[OK][process_manager] Process_manager with id %d waiting to produce %d elements.\n",

           p->id, p->num_products);

    

    // 3) Notificar al factory_manager que estamos listos

    sem_post(&p->ready_sem[p->index]);

    

    // 4) Esperar la señal para comenzar la producción

    sem_wait(&p->start_sem[p->index]);

    

    // 5) Inicializar mutex, condvars y belt

    pthread_mutex_init(&mutex, NULL);

    pthread_cond_init(&not_full, NULL);

    pthread_cond_init(&not_empty, NULL);

    

    if (queue_init(p->belt_size) != 0) {

        fprintf(stderr, "[ERROR][process_manager] There was an error executing process_manager with id %d.\n", p->id);

        sem_post(&p->done_sem[p->index]);

        pthread_exit(NULL);

    }

    

    printf("[OK][process_manager] Belt with id %d has been created with a maximum of %d elements.\n",

           p->id, p->belt_size);

    

    // 6) Crear hilos productor y consumidor

    pthread_t prod, cons;

    pthread_create(&prod, NULL, producer, p);

    pthread_create(&cons, NULL, consumer, p);

    

    // 7) Esperar a que terminen

    pthread_join(prod, NULL);

    pthread_join(cons, NULL);

    

    queue_destroy();

    

    // 8) Imprimir mensaje de finalización

    printf("[OK][process_manager] Process_manager with id %d has produced %d elements.\n",

           p->id, p->num_products);

    

    // 9) Notificar al factory_manager que ha terminado

    sem_post(&p->done_sem[p->index]);

    

    // 10) Limpiar recursos

    pthread_mutex_destroy(&mutex);

    pthread_cond_destroy(&not_full);

    pthread_cond_destroy(&not_empty);

    

    pthread_exit(NULL);

}
