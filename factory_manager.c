/*
 *
 * factory_manager.c
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stddef.h>
#include <semaphore.h>
#include <sys/stat.h>

const int max_line = 1024;

int main (int argc, const char * argv[] ){
	if (argc != 2){
		fprintf(stderr, "Debes especificar el archivo del que leer");
		return -1;
	}

	int* status;
	int fd = open(argv[1], O_RDONLY); /*Abirmos el archivo del que leer la cadena*/
	if (fd != -1 ){
		fprintf(stderr, "Error al abrir el archivo");
	}

	char buffer[max_line]; /*Creo la cadena de caracteres buffer, de la cual leeremos los /n */
    ssize_t bytes_leidos;
	int cantidad_numeros = -2; /*Se inicializa en -2 para tener en cuenta el /n y el numero de los procces manager*/
	int contador_numeros = 0;

	while ((bytes_leidos = read(fd, buffer, 1)) > 0) {
		contador_numeros ++;
    }
	if ((contador_numeros% 3) == 0){
		printf("correcto");
	}


	return 0;
}
