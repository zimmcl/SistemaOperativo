/*
 * manipulacion_archivos.c
 *
 *      Author: Ceballos, Matias
 *      		Zimmel, Ezequiel
 */

#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include "../include/manipulacion_archivos.h"

//----------------------------------------------------------------
/**
 * @brief Obtener un valor desde archivo en /proc
 * @param archivo - archivo de busqueda
 * @param valor - resultado
 * @param llave - a buscar.
 * @return no aplica
 */

void getValor(char* archivo, char* valor, char* llave){
	char buffer[500];
	char* match = NULL;
	FILE* fd;

	fd = fopen(archivo,"r");
	while(feof(fd)==0){
		fgets(buffer, 500, fd);
		match = strstr(buffer, llave);
		if(match!=NULL)
			break;
	}

	fclose(fd);
	strcpy(valor,match);

	return;
}

//----------------------------------------------------------------
/**
 * @brief Modifica la salida estándar. Usado al implemtar pipe
 * @param fileName Path al archivo donde se envía la salida.
 * @return 1 si fue exitoso. 0 si se produjo error.
 */
int out_Redirect(char fileName[]){
	int fid;
	int flags,perm;
	flags = O_WRONLY|O_CREAT|O_TRUNC;
	perm = S_IWUSR|S_IRUSR;

	fid = open(fileName, flags, perm);
	if (fid<0) {
		perror("open");
		return 0;
	}
	close(STDOUT_FILENO);
	if (dup(fid)<0) {
		perror("dup");
		return 0;
	}
	close(fid);
	return 1;
}

//----------------------------------------------------------------
/**
 * @brief Modifica la entrada estándar. Usado al implemtar pipe
 * @param fileName Path al archivo donde se envía la salida.
 * @return fid si fue exitoso. 0 si se produjo error.
 */
int in_Redirect(char fileName[]){
	int fid;
	int flags,perm;
	flags = O_RDONLY;
	perm = S_IWUSR|S_IRUSR;

	close(STDIN_FILENO);
	fid = open(fileName, flags, perm);
	if (fid<0) {
		perror("open");
		return 0;
	}
	if (dup(fid)<0) {
		perror("dup");
		return 0;
	}
	//close(fid);
	return fid;
}

//----------------------------------------------------------------
/**
 * @brief Devuelve el tamaño de la linea de comandos ingresada por el usuario.
 * @param argv[] Cadena de argumentos. Modificada por referencia
 * @param cadena Cadena ingresada por el usuario
 * @return words Tamaño de la linea ingresada
 */
int read_input(char* argv[], char* cadena){
	int words = 0;

	if(feof(stdin))			//Verifica si se ingresa Ctrl+D
			    {
			printf("\n");
			exit (0);
			    }

	argv[0] = strtok(cadena, " \n");
	for(words = 1; words < 20; words++){
		argv[words] = strtok(NULL, " \n");
		if (argv[words] == NULL)
			break;
	}
	return words;
}
