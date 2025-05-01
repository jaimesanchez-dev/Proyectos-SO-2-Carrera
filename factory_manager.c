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

struct parametros {
    int id;
    int belt_size;
    int num_products;
};


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
        lista_parametros[i].num_products = numeros[j++]; printf("%d", numeros[j]);
    }
}