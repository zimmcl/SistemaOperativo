/*
 * sistema_info.c
 *
 *  Created on: 02/09/2016
 *      Author: Ezequiel
 */
#include"../include/manipulacion_archivos.h"
#include"../include/sistema_info.h"
#include<stdio.h>
#include<string.h>
#include<time.h>

#define SIZE 256

/**
 * Imprime tiempo activo del ordenador en formato h:m:s
 */

void tiempo_activo(){
	float tiempo;
	int hora,minuto,segundo;

	FILE *fd;
	fd=fopen("/proc/uptime","r");
	fscanf(fd,"%f", &tiempo);
	fclose(fd);
	hora=(int)(tiempo / 3600);
	minuto=(int)((tiempo - hora * 3600) / 60);
	segundo=tiempo - (hora * 3600 + minuto * 60);
	printf("Tiempo Activo : %dh %dm %ds",hora,minuto,segundo);

	return;
}

/**
 * Cabecera
 */

void cabecera(){
	hostname();
	fecha_actual();

	return;
}

/**
 * Imprime nombre de PC
 */

void hostname(){
	char nombre[25];
	FILE *fd;

	fd = fopen("/proc/sys/kernel/hostname","r");
	fscanf(fd, "%[^\n]s", nombre);
	printf("Nombre Equipo: %s ", nombre);
	fclose(fd);

	return;
}

/**
 * Imprime fecha y hora actual
 */

void fecha_actual(){
	char fecha[12];
	char valor[SIZE+1];
	char hora[13];

	getValor("/proc/driver/rtc", valor, "rtc_date");
	sscanf(valor, "rtc_date : %s", fecha);
	getValor("/proc/driver/rtc", valor, "rtc_time");
	sscanf(valor, "rtc_time : %s", hora);
	printf("   Fecha: %s - Hora: %s", fecha, hora);

	return;
}

/**
 * Imprime la memoria Total y Libre en MB (KB)
 */

void memoria_info(int h){
	unsigned int memlibre;
	unsigned int memtotal;
	char valor[SIZE];

	getValor("/proc/meminfo", valor, "MemTotal");
	sscanf(valor, "MemTotal : %u", &memtotal);
	getValor("/proc/meminfo", valor, "MemFree");
	sscanf(valor, "MemFree : %u", &memlibre);
	printf("MEMORIA\n    -Memoria Total: %u MB (%u KB)\n    -Memoria Libre: %u MB (%u KB)", memtotal/1024, memtotal, memlibre/1024, memlibre);

	return;
}

/**
 * Documentación: https://www.kernel.org/doc/Documentation/iostats.txt
 * Campo 1 Número total de lecturas completadas satisfactoriamente.
 * Campo 5 Número total de escrituras completadas satisfactoriamente.
 *
 * Imprime las lecturas, escritura y R+W completadas satisfactoriamente
 */

void hdd_info(){
	unsigned int  basura, lectura, escritura;
	char valor[SIZE];

	getValor("/proc/diskstats", valor,"sda");
	sscanf(valor,"sda %u %u %u %u %u", &lectura, &basura, &basura, &basura, &escritura);
	printf("DISCO\n    -Pedidos de lectura: %u\n    -Pedidos de escritura: %u\n    -Total R+W: %u", lectura, escritura, lectura+escritura);

	return;
}

/**
 * Documentación: http://man7.org/linux/man-pages/man5/proc.5.html
 *
 * Imprime los cambios de contexto
 */

void contexto(){
	unsigned int contexto;
	char valor[SIZE];

	getValor("/proc/stat", valor, "ctxt");
	sscanf(valor,"ctxt %u",&contexto);
	printf("Cambios de Contexto: %u",contexto);

	return;
}

/**
 *Imprime procesos creados desde el inicio
 */

void procesos(){
	unsigned int procesos;
	char valor[SIZE];

	getValor("/proc/stat", valor, "processes");
	sscanf(valor,"processes %u",&procesos);
	printf("Procesos: %u",procesos);

	return;
}

/**
 * Imprime la fecha de inicio del ordenador
 */

void tiempo_inicio(){
	char valor[SIZE];
	unsigned int tiempo;
	time_t btime;
	char fecha[50];

	getValor("/proc/stat", valor, "btime");
	sscanf(valor, "btime %u", &tiempo);
	btime= (time_t)tiempo;
	strftime(fecha,sizeof(fecha),"%c", localtime(&btime));
	printf("Fecha Inicio : %s", fecha);

	return;
}

/**
 * Imprime el promedio de carga
 */

void carga(){
	FILE *fd;
	float carga;
	fd = fopen("/proc/loadavg","r");
	fscanf(fd,"%f", &carga);
	printf("Promedio de carga de un minuto: %f", carga);
	fclose(fd);
	return;
}
