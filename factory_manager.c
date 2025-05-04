/*
 * factory_manager.c
 */

 #include <stdio.h>
 #include <stdlib.h>
 #include <unistd.h>
 #include <fcntl.h>
 #include <string.h>
 #include <pthread.h>
 #include <semaphore.h>
 #include "process_manager.h"  // must define struct parametros with id, belt_size, num_products, index, start_sem, done_sem
 
 #define MAX_LINE 1024
 
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
     buffer[bytes_read] = '\0'; // terminate
 
     close(fd);
 
     // tokenize numbers
     int numeros[MAX_LINE/4];
     int countador = 0;
     char *token = strtok(buffer, " \n");
     while (token != NULL && countador < MAX_LINE/4) {
         numeros[countador++] = atoi(token);
         token = strtok(NULL, " \n");
     }
 
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
 
     // allocate parameter array and threads
     struct parametros *lista_parametros = malloc(sizeof(*lista_parametros) * num_cintas);
     pthread_t *threads = malloc(sizeof(*threads) * num_cintas);
 
     // allocate & init per-process semaphores
     sem_t *start_sem = malloc(sizeof(*start_sem) * num_cintas);
     sem_t *done_sem  = malloc(sizeof(*done_sem)  * num_cintas);
     for (int i = 0; i < num_cintas; i++) {
         sem_init(&start_sem[i], 0, 0);
         sem_init(&done_sem[i],  0, 0);
     }
 
     // fill in parametros (including index + semaphore pointers)
     int j = 1;
     for (int i = 0; i < num_cintas; i++) {
         lista_parametros[i].id           = numeros[j++];
         lista_parametros[i].belt_size    = numeros[j++];
         lista_parametros[i].num_products = numeros[j++];
         lista_parametros[i].index        = i;
         lista_parametros[i].start_sem    = start_sem;
         lista_parametros[i].done_sem     = done_sem;
     }
 
     // create all process_manager threads
     for (int i = 0; i < num_cintas; i++) {
         if (pthread_create(&threads[i], NULL,
                            process_manager,
                            &lista_parametros[i]) != 0) {
             fprintf(stderr,
                     "[ERROR][factory_manager] Process_manager with id %d has finished with errors.\n",
                     lista_parametros[i].id);
         }
         else {
             printf("[OK][factory_manager] Process_manager with id %d has been created.\n",
                    lista_parametros[i].id);
         }
     }
 
     // sequentially start and wait each process_manager
     for (int i = 0; i < num_cintas; i++) {
         // allow the i-th process to begin
         sem_post(&start_sem[i]);
         // wait until it signals completion
         sem_wait(&done_sem[i]);
         printf("[OK][factory_manager] Process_manager with id %d has finished.\n",
                lista_parametros[i].id);
     }
 
     // join threads to clean up
     for (int i = 0; i < num_cintas; i++) {
         pthread_join(threads[i], NULL);
     }
 
     printf("[OK][factory_manager] Factory manager has finished.\n");
 
     // destroy semaphores and free memory
     for (int i = 0; i < num_cintas; i++) {
         sem_destroy(&start_sem[i]);
         sem_destroy(&done_sem[i]);
     }
     free(start_sem);
     free(done_sem);
     free(lista_parametros);
     free(threads);
 
     return 0;
 }
 