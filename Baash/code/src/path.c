/*
 * path.c
 *
 *  Created on: 19/10/2016
 *      Author: Ezequiel
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFERSIZE 256
#define DELIM ":"

char **get_path_entries() {
	int bufsize = BUFFERSIZE, pos=0;
	/* Guardo el contenido de la variable de entorno PATH. */
	char *path;
	path = getenv("PATH");
	/* Array que almacena las rutas de la variable PATH */
	char **path_array = malloc(sizeof(char) * bufsize);

	if (!path_array) {
		    fprintf(stderr, "lsh: Error en la reserva de memoria\n");
		    exit(EXIT_FAILURE);
		  }

	/* Divido en ":" y encuentro todos las rutas incluidas en PATH. */
	char *token;
	char *divider = DELIM;

	token = strtok (path, divider);

	/* Hasta que no haya más rutas */
	while (token != NULL) {

		if (pos >= bufsize) {
				bufsize += BUFFERSIZE;
				path_array = realloc(path_array, bufsize * sizeof(char*));
				if (!path_array) {
				     fprintf(stderr, "lsh: Error en la reserva de memoria\n");
				     exit(EXIT_FAILURE);
				  	  }
			}

		/* Guardo la ruta en "path_array" */
		strcpy(path_array[pos], token);
		pos++;

		/* Busco el próximo token */
		token = strtok (NULL, divider);
	}
	path_array[pos]=NULL;
	return path_array;
}
