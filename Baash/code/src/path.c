/*
 * path.c
 *
 *      Author: Ceballos, Matias
 *      		Zimmel, Ezequiel
 */

#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "../include/path.h"

#define BUFFERSIZE 256
#define PATH_MAX 64
#define DELIM ":"

//----------------------------------------------------------------
/**
   @brief Obtiener los directorios que almacena la variable de entorno PATH.
   @param paths[] Arreglo de punteros a caracter donde se almacena cada path.
   @return cantidad de direcctorios de la variable de entorno PATH.
 */
int get_path_entries(char* path[]) {
	int pos=0;
	/* Guardo el contenido de la variable de entorno PATH. */
	char *pathEn;
	pathEn = getenv("PATH");

	/* Divido en ":" y encuentro todos las rutas incluidas en PATH. */
	char *token;
	token = strtok (pathEn, DELIM);

	/* Hasta que no haya más rutas */
	while (token != NULL) {
		/* Guardo la ruta en "path" */
		path[pos] = token;
		pos++;

		/* Busco el próximo token */
		token = strtok (NULL, DELIM);
	}
	path[pos]='\0';
	return pos;
}

//----------------------------------------------------------------
/**
   @brief Construye el directorio del programa a ejecutar, ya sea por
   directorios relativos o directorios absolutos.
   @param arch Nombre del programa
   @param paths[] Variable de entorno PATH
   @param excecvPath Directorio a ejecutar (modificado por referencia)
   @return no aplica.
 */
void search_File_Dir(char* arch, char* paths[], char *execvPath){
	int result=0;
	char searchDir[50] = "";
	char* archivo;

	//Sector donde se discrimina entre directorios relativos o absolutos
	if(arch[0] == '/' || (arch[0] == '.' && arch[1] == '.' && arch[2] == '/')){
		char* dir;
		char* nextDir;
		int pathReady = 0; //Bandera que controla si ya se obtuvo el path completo

		if(arch[0] == '/')   //Se pregunta si el directorio es absoluto
			searchDir[0] = '/';
		dir = strtok(arch,"/");
		nextDir = strtok(NULL,"/");

		if(nextDir != NULL){ //Si el comando no es de la forma /archivo
		strcat(searchDir,dir);
		//printf("El parth es: %s\n", searchDir);
		}
		else{
			nextDir = dir; //Si archivo es NULL, entonces pongo en archivo lo que hay despues de la ultima /
			pathReady = 1;
		}

		while((nextDir != NULL) && !pathReady){
			dir = nextDir;
			nextDir = strtok(NULL,"/");
			strcat(searchDir,"/");
			if(nextDir != NULL)
				strcat(searchDir,dir);
		}
		archivo = dir;
	}

	else if(arch[0] == '.' && arch[1] == '/'){ //Es un path relativo, tomando como path el directorio actual.
		getcwd(searchDir, 50);
		strcat(searchDir,"/");
		archivo = strtok(arch, "/");
		archivo = strtok(NULL,"/");
	}

	else{
		//Tiene que buscar en todos los directorios del path.
		//Se copian los directorios incluidos en PATH en la variable execvPath
		int i;
		char *aux = malloc(sizeof(char) * BUFFERSIZE);
		for(i = 0; i < 20; i++){
			if(paths[i] == NULL)
				break;
			strcpy(aux,paths[i]);
			strcat(aux,"/");
			strcat(aux,arch);
			result = access(aux, F_OK);  //Verifica si se puede acceder al file
			if(!result){
				strcpy(execvPath, aux);
				free(aux);
				return;
			}
		}
		execvPath[0] = 'X';  //bandera para indicar un nombre de programa incorrecto
		free(aux);
		return;
	}
	strcat(searchDir, archivo);
	result = access(searchDir, F_OK);
	if(!result)
		strcpy(execvPath, searchDir);
	else
		execvPath[0] = 'X';
}

//----------------------------------------------------------------
/**
 * @brief Verifica si se debe redireccionar la entrada o la salida estandar.
 * @param argv Arreglo que contiene el comando y los argumentos ingresados.
 * @param fileName Almacena el nombre del archivo del que se lee, o en el que se escribe la salida.
 * @return Devuelve 0 si no hay que redireccionar, 1 si hay que redireccionar la entrada, y 2 si hay que redireccionar la salida.
 */
int check_Redirect(char* argv[], char fileName[]){
	int i;
	for (i = 0; i < 20; i++){

		if(argv[i] == NULL){
			fileName = NULL;
			return 0;
		}
		else if (!strcmp(argv[i], "<")){
			strcpy(fileName, argv[i+1]);
			argv[i] = NULL;
			argv[i+1] = NULL;
			return 1;
		}
		else if (!strcmp(argv[i], ">")){
			strcpy(fileName, argv[i+1]);
			argv[i] = NULL;
			argv[i+1] = NULL;
			return 2;
		}
	}
	return 0;
}

//----------------------------------------------------------------
/**
 * @brief Verifica si debe realizarse un pipeline.
 * @param argv Argumentos del comando ingresado por el usuario.
 * @param argv1 Array donde se guardarán los argumentos del comando 1.
 * @param argv2 Array donde se guardarán los argumentos del comando 2.
 * @return Devuelve 1 si se debe ejecutar el pipeline. 0 en caso contrario.
 */
int check_Pipe(char* argv[], char* argv1[], char* argv2[]){
	int indexArg, aux, indexArg2;

	for(indexArg = 0; argv[indexArg] != NULL; indexArg++){
		aux = strcmp("|", argv[indexArg]);
		if(aux == 0)
			break;
		argv1[indexArg] = (char*) malloc ( strlen(argv[indexArg] + 1) ) ;
		strcpy(argv1[indexArg], argv[indexArg]);
	}
	argv1[indexArg] = '\0';

	//Si no encontró |, se devuelve un 0, si no, se obtienen los dos argv de los comandos.
	if(argv[indexArg] == NULL)
		return 0;

	indexArg++;

	for(indexArg2 = 0; argv[indexArg] != NULL; indexArg2++){

		if(argv[indexArg] == NULL)
			break;
		argv2[indexArg2] = (char*) malloc ( strlen(argv[indexArg] + 1) ) ;
		strcpy(argv2[indexArg2], argv[indexArg]);
		indexArg++;
	}
	argv2[indexArg2] = '\0';

	return 1;
}

//----------------------------------------------------------------
/**
 * @brief Ejecuta el Pipeline
 * @param argv1 Argumentos del comando 1.
 * @param argv2 Argumentos del comando 2.
 * @param paths Paths donde puede buscar los archivos que ejecutan los comandos.
 */
void exec_Pipeline(char* argv1[], char* argv2[], char* paths[]){
	char executePath[256];

	int fd[2];
	pipe(fd);
	if (fork()==0) { //codigo del hijo
		close(fd[0]);
		dup2(fd[1],1); // redireccion de la salida al pipe.
		close(fd[1]);
		search_File_Dir(argv1[0], paths, executePath);
		execv(executePath, argv1);
		perror(executePath);
		exit(1);
	}
	else {
		close(fd[1]); //codigo del padre
		dup2(fd[0],0);
		close(fd[0]);
		search_File_Dir(argv2[0], paths, executePath);
		execv(executePath, argv2);
		perror(executePath);
		exit(1);
	}
}

