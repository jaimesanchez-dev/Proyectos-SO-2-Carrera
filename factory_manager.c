/*
 * factory_manager.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>
#include "process_manager.h"
#include <semaphore.h>

/*Struct de parametros movida a procces_manger.h*/

#define MAX_LINE 1024
extern sem_t Jamess;

int main(int argc, const char * argv[]) {
    if (argc != 2) {
        fprintf(stderr, "[ERROR][factory_manager] Invalid file.\n");
        return -1;
    }

    int fd = open(argv[1], O_RDONLY);
    if (fd == -1) {
        fprintf(stderr, "[ERROR][factory_manager] Invalid file.\n");
        return -1;
    }

    char buffer[MAX_LINE];
    ssize_t bytes_read = read(fd, buffer, MAX_LINE - 1);
    if (bytes_read <= 0) {
        fprintf(stderr, "[ERROR][factory_manager] Invalid file.\n");
        close(fd);
        return -1;
    }
    buffer[bytes_read] = '\0'; /*Para que funcione el Strtok*/
    close(fd);

    /*++++++++++++++++++++Procesar números+++++++++++++++++(inspirado del tokenizar linea del scripter)*/

    int numeros[MAX_LINE/4];    /*256 enteros*/
    int countador = 0;
    char *token = strtok(buffer, " \n");
    while (token != NULL && countador < MAX_LINE/4) {
        numeros[countador++] = atoi(token);
        token = strtok(NULL, " \n");
    }

	/*++++++++++++++++++++++++++++++++++++++++Procesar números++++++++++++++++++++++++++++++++++++++*/

    if (countador < 1 || (countador - 1) % 3 != 0) {
        fprintf(stderr, "[ERROR][factory_manager] Invalid file.\n");
        return -1;
    }

    int max_cintas = numeros[0];
    int num_cintas = (countador - 1) / 3;

    if (num_cintas > max_cintas || max_cintas <= 0) {
        fprintf(stderr, "[ERROR][factory_manager] Invalid file.\n");
        return -1;
    }

    struct parametros *lista_parametros = malloc(sizeof(struct parametros) * num_cintas);
    pthread_t *threads = malloc(sizeof(pthread_t) * num_cintas);

    int  j = 0;
    for (int i = 0; i < num_cintas; i++) {
        lista_parametros[i].id = numeros[j++]; printf("%d", numeros[j]);
        lista_parametros[i].belt_size = numeros[j++]; printf("%d", numeros[j]);
        lista_parametros[i].num_products = numeros[j++]; printf("%d\n", numeros[j]);
    }

    /*+++++++++++++++++++++++++++++++++Creacion de hilos++++++++++++++++++++++++++++++++++++*/

    sem_init(&Jamess, 0, 1); /*Inicializa el semaforo*/

    for (int i = 0; i < num_cintas; i++) {
        int creado = pthread_create(&threads[i], NULL, process_manager, (void*)&lista_parametros[i]);
        if (creado != 0) {
            /*Controla errores sino se crea*/
            fprintf(stderr, "[ERROR][factory_manager] Process_manager with id %d has finished with errors.\n", lista_parametros[i].id);
        } else {
            printf("[OK][factory_manager] Process_manager with id %d has been created.\n", lista_parametros[i].id);
        }
    }
    /*+++++++++++++++++++++++++++++++++Creacion de hilos++++++++++++++++++++++++++++++++++++*/


    /*+++++++++++++++++++++++++++++++++Espera de hilos++++++++++++++++++++++++++++++++++++*/

    for (int i = 0; i < num_cintas; i++) {
        int recogido = pthread_join(threads[i], NULL);
        if (recogido != 0) {
            fprintf(stderr, "[ERROR][factory_manager] Process_manager with id %d has finished with errors.\n", lista_parametros[i].id);
        } else {
            printf("[OK][factory_manager] Process_manager with id %d has finished.\n", lista_parametros[i].id);
        }
    }

    sem_destroy(&Jamess); /*Destruye el semaforo*/
    free(lista_parametros);
    free(threads);
    printf("[OK][factory_manager] Factory manager has finished.\n");
    return 0;

    /*+++++++++++++++++++++++++++++++++Espera de hilos++++++++++++++++++++++++++++++++++++*/
}