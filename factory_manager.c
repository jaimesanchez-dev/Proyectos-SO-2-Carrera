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
 
 struct parametros{
	 int id;
	 int belt_size;
	 int num_products;
 };
 
 const int max_line = 1024;
 
 int main (int argc, const char * argv[] ){
	 if (argc != 2){
		 fprintf(stderr, "[ERROR][factory_manager] Invalid file.\n");
		 return -1;
	 }
 
	 int* status;
	 int fd = open(argv[1], O_RDONLY); /*Abirmos el archivo del que leer la cadena*/
	 if (fd == -1 ){
		 fprintf(stderr, "[ERROR][factory_manager] Invalid file.\n");
		 return -1;
	 }
 
	 char buffer[max_line]; /*Creo la cadena de caracteres buffer, de la cual leeremos los /n */
	 ssize_t bytes_leidos;
	 int contador_numeros = -2; /*Se inicializa en -2 para tener en cuenta el /n y el numero de los procces manager*/
	 int procesos = 0; /*Numero de procces managers a crear*/
 
	 while ((bytes_leidos = read(fd, buffer, 1)) > 0) {
		 contador_numeros ++;
		 
	 }
	 if ((contador_numeros% 3) == 0){
		 printf("correcto");
		 procesos = contador_numeros/3;
	 } else {
		 return -1;
	 };
 
	 struct parametros *lista_parametros = malloc(sizeof(struct parametros) * procesos); /*Asigno la momoria que voy a utilizazr para pasar los parametros*/
	 pthread_t *threads = malloc(sizeof(pthread_t) * procesos);
 
	 
 
 
	 return 0;
 }
 