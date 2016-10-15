/*
 * manipulacion_archivos.c
 *
 *  Created on: 12/10/2016
 *      Author: Ezequiel
 */

#include <string.h>
#include <stdio.h>
#include "../include/manipulacion_archivos.h"

/**
 * Obtener un valor desde archivo en /proc
 * @param archivo - archivo de busqueda
 * @param valor - resultado
 * @param llave - a buscar.
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
