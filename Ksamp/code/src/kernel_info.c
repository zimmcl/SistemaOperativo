/*
 * kernel-info.c
 *
 *  Created on: 27/08/2016
 *      Author: Ezequiel
 */
#include"../include/manipulacion_archivos.h"
#include"../include/kernel_info.h"
#include<stdio.h>
#include<string.h>

#define SIZE 256

/**
 * Imprime version del Kernel
 */

void kernel_version(){
	char version[50];
	FILE *fd;

	fd = fopen("/proc/version","r");
	fscanf(fd, "%[^(]s", version);
	printf("KERNEL - Version: %s", version);
	fclose(fd);

	return;
}

/**
 * Imprime cantidad de archivos de sistema soportado
 */

void sistema_archivo(){
	int cantidad=0;
	char cadena[100];

	FILE *fd;
	fd=fopen("/proc/filesystems","r");
	while(fgets(cadena, 100, fd) != NULL){
		cantidad++;
		}
	fclose(fd);
	printf("KERNEL - Nro de FS : %d" ,cantidad);

	return;
}


