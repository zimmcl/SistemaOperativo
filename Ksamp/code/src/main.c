/*
 * main.c
 *
 *  Created on: 27/08/2016
 *      Author: Ezequiel
 */
#include "../include/kernel_info.h"
#include "../include/cpu_info.h"
#include "../include/sistema_info.h"
#include "../include/manipulacion_archivos.h"

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

void parteB();
void parteC();
void parteD(int intervalo, int duracion);

/*
 * Documentación: http://www.gnu.org/software/libc/manual/html_node/Example-of-Getopt.html
 */

int main (int argc, char *argv[])
{
    int arg1, arg2, c;
    short int flagC = 0;
    short int flagD = 0;

    while((c = getopt (argc, argv, "sl:"))!=-1){
		switch(c){
			case 's':
				if(argc==2 && (strcmp(argv[1],"-s")==0)){
					flagC = 1;
					break;
				}
				return 0;
			case 'l':
				if(argc == 4){
					//flagC = 1;
					flagD = 1;
					break;
				}
				return 0;
			default:
				printf("Opciones: -s , -l arg1 arg2\n");
				return 0;
		}
	}

    cabecera();
    printf("\n");
    parteB();

	if(flagC)
		parteC();
	if(flagD){
		sscanf(argv[argc-2], "%d", &arg1);
		sscanf(argv[argc-1], "%d", &arg2);
		//parteC();
		parteD(arg1,arg2);
	}
	return 0;
}

/**
 * CONSIGNA PARTE B
 */

void parteB(){
	cpu_modelo();printf("\n");
	cpu_tipo();printf("\n");
	kernel_version();printf("\n");
        sistema_archivo();printf("\n");
	tiempo_activo();printf("\n");
	return;
}

/**
 * CONSIGNA PARTE C
 */

void parteC(){
	tiempo_uso();printf("\n");
	contexto();printf("\n");
	tiempo_inicio();printf("\n");
	procesos();printf("\n");
	return;
}

/**
 * CONSIGNA PARTE D
 * @param intervalo: intervalo de muestreo
 * @param duracion: tiempo de muestreo
 */

void parteD(int intervalo, int duracion){
	int i;
	i=duracion/intervalo;
	printf("-----------------------------------------\n");
	parteC();
	printf("-----------------------------------------\n");
	while(i>0){
		memoria_info();printf("\n");
		hdd_info();printf("\n");
		carga();printf("\n");
		i--;
		sleep(intervalo);
		printf("-----------------------------------------\n");
	}
	return;
}


