/*
 * cpu-info.c
 *
 *  Created on: 27/08/2016
 *      Author: Ezequiel
 */
#include"../include/manipulacion_archivos.h"
#include"../include/cpu_info.h"
#include<stdio.h>

#define SIZE 256

/**
 * Imprime modelo de CPU
 */

void cpu_modelo(){
	char modelo[100];
	char valor[SIZE];

	getValor("/proc/cpuinfo", valor, "model name");
	sscanf(valor, "model name : %[^\n]s", modelo);
	printf("CPU - Modelo: %s", modelo);

	return;
}

/**
 * Imprime tipo de CPU
 */

void cpu_tipo(){
	char tipo[50];
	char valor[SIZE];

	getValor("/proc/cpuinfo", valor, "vendor_id");
	sscanf(valor, "vendor_id : %s", tipo);
	printf("CPU - Tipo: %s", tipo);

	return;
}

/**
 * Imprime cantidad de nucleos
 */

void cpu_nucleos(){
	int nucleos;
	char valor[SIZE];

	getValor("/proc/cpuinfo", valor, "cpu cores");
	sscanf(valor, "cpu cores : %d", &nucleos);
	printf("CPU - Nucleos: %d", nucleos);

	return;
}
/*
 * Documentación: https://supportcenter.checkpoint.com/supportcenter/portal?eventSubmit_doGoviewsolutiondetails=&solutionid=sk65143
 * Campo 1 Tiempo Usuario
 * Campo 3 Tiempo Sistema
 * Campo 4 Tiempo Idle
 */

void tiempo_uso(){
	int usuario,sistema,idle, basura;
	int horaU, horaS, horaI, minU, minS, minI, segU, segS, segI;
	char valor[SIZE];

	getValor("/proc/stat", valor, "cpu");
	sscanf(valor,"cpu %d %d %d %d", &usuario, &basura, &sistema, &idle);

	horaU=(int)((usuario/100) / 3600);
	minU=(int)(((usuario/100) - horaU * 3600) / 60);
	segU=(usuario/100) - (horaU * 3600 + minU * 60);

	horaS=(int)((sistema/100) / 3600);
	minS=(int)(((sistema/100) - horaS * 3600) / 60);
	segS=(sistema/100) - (horaS * 3600 + minS * 60);

	horaI=(int)((idle/1000) / 3600);
	minI=(int)(((idle/1000) - horaI * 3600) / 60);
	segI=(idle/1000) - (horaI * 3600 + minI * 60);
	printf("Tiempo de uso CPU\n");
	printf("    -Usuario: %dh %dm %ds\n",horaU,minU,segU);
	printf("    -Sistema: %dh %dm %ds\n",horaS,minS,segS);
	printf("    -Idle: %dh %dm %ds (*)",horaI,minI,segI);

	return;
}
