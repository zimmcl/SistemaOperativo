/*
 * builtin.c
 *
 *  Created on: 13/10/2016
 *      Author: Ezequiel
 */


#include <unistd.h>
#include <ncurses.h>
#include <stdio.h>
#include "../include/builtin.h"
#include "../include/prompt.h"

/*
  Lista de comandos builtin.
 */
char *builtin_str[] = {
  "cd",
  "help",
  "exit",
  "clear",
  "pwd"
};

char *builtin_desc[] = {
  "Cambiar directorio de trabajo a directorio pasado como parámetro.",
  "Imprimir texto de ayuda de todos los comandos internos",
  "Finalizar el shell actual en ejecución.",
  "Limpiar la pantalla.",
  "Mostrar nombre de ruta absoluta del directorio de trabajo actual."
};

int (*builtin_func[]) (char **) = {
  &lsh_cd,
  &lsh_help,
  &lsh_exit,
  &lsh_clear,
  &lsh_pwd
};

int lsh_num_builtins(void) {
  return sizeof(builtin_str) / sizeof(char *);
}

/*
   Implementación de las funcines builtin.
*/
//----------------------------------------------------------------
/**
   @brief Comando Builtin: Cambiar de directorio.
   @param args Lista de argumentos.  args[0] "cd".  args[1] es el directorio.
   @return Siempre retorna 1 para continuar ejecutando.
 */
int lsh_cd(char **args)
{
  if (args[1] == NULL) {
    fprintf(stderr, "lsh: espera argumento para \"cd\"\n");
  } else {
    if (chdir(args[1]) != 0) {
      perror("Error: ");
    }
  }
  return 1;
}

//----------------------------------------------------------------
/**
   @brief Comando Builtin: Imprimir ayuda.
   @param args Lista de argumentos.  No aplica.
   @return Siempre retorna 1 para continuar ejecutando.
 */
int lsh_help(char **args)
{
  int i;
  printf("AYUDA.\n");
  printf("Comandos internos:\n");

  for (i = 0; i < lsh_num_builtins(); i++) {
    printf("  %s:  %s\n", builtin_str[i], builtin_desc[i]);
  }

  printf("Use el comando man para información de otros programas.\n");
  return 1;
}

//----------------------------------------------------------------
/**
   @brief Comando Builtin: Salir.
   @param args Lista de argumentos.  No aplica.
   @return Siempre retorna 0 para terminar ejecución.
 */
int lsh_exit(char **args)
{
  return 0;
}

//---------------------------------------------------------------
/**
   @brief Comando Builtin: Limpiar pantalla.
   @param args Lista de argumentos.  No aplica.
   @return Siempre retorna 1 para continuar ejecutando.
 */
int lsh_clear(char **args)
{
	system("clear");
	return 1;
}

//---------------------------------------------------------------
/**
   @brief Comando Builtin: Imprime la ruta absoluta del directorio de trabajo actual.
   @param args Lista de argumentos.  No aplica.
   @return Siempre retorna 1 para continuar ejecutando.
 */
int lsh_pwd(char **args)
{
	char cwd[1024];
	getcwd(cwd, sizeof(cwd));
	fprintf(stdout,"%s\n", cwd);

	return 1;
}
