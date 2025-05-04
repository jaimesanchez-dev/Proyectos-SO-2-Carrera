#ifndef PROCESS_MANAGER_H

#define PROCESS_MANAGER_H



#include <semaphore.h>



struct parametros {

    int id;

    int belt_size;

    int num_products;



    int index;               // Índice del proceso

    sem_t *start_sem;        // Semáforos para arrancar procesos

    sem_t *done_sem;         // Semáforos para indicar que han terminado

};



void* process_manager(void* arg);



#endif

