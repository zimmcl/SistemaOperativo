/*
 * prompt.c
 *
 *      Author: Ceballos, Matias
 *      		Zimmel, Ezequiel
 */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pwd.h>
#include "../include/manipulacion_archivos.h"
#include "../include/prompt.h"

#define LIGHT_CYAN "\033[1;36m"
#define COLOR_RESET "\x1b[0m"

//----------------------------------------------------------------
/**
   @brief Funcion encargada de mostrar el prompt del bash.
   @param no aplica
   @return no aplica
 */
void prompt(void)
{
	int valor;
	tiempo_actual();
	valor = whoami();
	hostname();
	directorio();
	if(valor==0){
	printf("# "COLOR_RESET);}
	else printf("$ "COLOR_RESET);
	return;
}

//----------------------------------------------------------------
/**
   @brief Obtiene el nombre del equipo
   @param no aplica
   @return no aplica
 */
void hostname(void){
	char nombre[25];
	FILE *fd;

	fd = fopen("/proc/sys/kernel/hostname","r");
	fscanf(fd, "%[^\n]s", nombre);
	printf("%s:", nombre);
	fclose(fd);

	return;
}

//----------------------------------------------------------------
/**
   @brief Obtiene tiempo actual
   @param no aplica
   @return no aplica
 */
void tiempo_actual(void){
	char valor[256];
	char hora[13];

	getValor("/proc/driver/rtc", valor, "rtc_time");
	sscanf(valor, "rtc_time : %s", hora);
	printf(LIGHT_CYAN "[%s]", hora);

	return;
}

//----------------------------------------------------------------
/**
   @brief Obtiene el tipo de usuario
   @param no aplica
   @return devuelve 0 para usuario root. !=0 para otro tipo de usuario
 */
int whoami(void) {

          struct passwd *pw;
          char *user = NULL;

          pw = getpwuid(geteuid());
          if (pw)
               user = pw->pw_name;
          else if ((user = getenv("USER")) == NULL) {
               fprintf(stderr, "No tengo idea!\n");
               return 1;
          }
          printf("%s@", user);
          return geteuid();
}

//----------------------------------------------------------------
/**
   @brief Muestra el directorio actual
   @param no aplica
   @return no aplica
 */
void directorio(void)
{
	char *current_dir;
	char cwd[1024];
	char *home_var = getenv("HOME");

	if (getcwd(cwd, sizeof(cwd)) != NULL){
		current_dir = cwd;

		if(strncmp(cwd, home_var, strlen(home_var)) == 0) {
					current_dir = current_dir + strlen(home_var);

					printf("~%s", current_dir);
				} else {

					printf("%s", current_dir);
				}
	}
	else
	    perror("getcwd() error: ");
	return;
}
