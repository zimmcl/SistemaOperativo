/*
 * main.c
 *
 *  Created on: 11/10/2016
 *      Author: Ezequiel
 */

#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pwd.h>
#include <sys/types.h>
#include <errno.h>
#include "../include/builtin.h"
#include "../include/manipulacion_archivos.h"
#include "../include/prompt.h"

#define RL_BUFFSIZE 1024
#define TOK_BUFFSIZE 64
#define PATHLENGTH 32

#define TOK_DELIM " "
#define LIGHT_CYAN "\033[1;36m"
#define COLOR_RESET "\x1b[0m"

char *operadores[4] = {">","<","|",">>"};

/*
  Declaración de Funciones:
 */

void ChildProcess(char **args);
void ParentProcess(pid_t pid, int status);

//----------------------------------------------------------------
/**
  @brief Lanza un programa y espera a que termine.
  @param
  @return Siempre retorna 1 para continuar ejecutando.
 */
int lsh_launch(char **args)
{
  pid_t pid;
  int status=0;
  pid = fork();
  if (pid == 0) {
    ChildProcess(args);
  } else if (pid < 0) {
    // Error en fork()
    perror("Error");
  } else {
    ParentProcess(pid,status);
  }

  return 1;
}

//----------------------------------------------------------------
/**
   @brief
   @param
   @return
 */
void ChildProcess(char **args)
{
	if (execv(args[0], args) == -1) {
	      perror("Error");
	      printf("Use help para ver los comandos internos disponibles.\n");
	    }
	    exit(EXIT_FAILURE);
}

//----------------------------------------------------------------
/**
   @brief
   @param
   @return
 */
void ParentProcess(pid_t pid, int status)
{
	do {
	      waitpid(pid, &status, WUNTRACED);
	    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
}

//----------------------------------------------------------------
/**
   @brief
   @param
   @return
 */
int lsh_execute(char **args, char **operadores)
{
  int i, j=0;

  if (args[0] == NULL) {
    // Se introdujo un comando vacio
    return 1;
  }

  while(operadores[j]!=NULL)
  {
	  if(!strcmp(operadores[j],">"))
	  {
		  //operacion1();
		  return 1;
	  }
	  if(!strcmp(operadores[j],"<"))
	  {
	  	  //operacion2();
		  return 1;
	  }
	  if(!strcmp(operadores[j],">>"))
	  {
	  	  //operacion3();
		  return 1;
	  }
	  if(!strcmp(operadores[j],"|"))
	  {
	  	  //operacion4();
		  return 1;
	  }
  }

  for (i = 0; i < lsh_num_builtins(); i++) {
    if (strcmp(args[0], builtin_str[i]) == 0) {
      return (*builtin_func[i])(args);
    }
  }

  return lsh_launch(args);
}

//----------------------------------------------------------------
/**
   @brief Leer una línea de entrada de la entrada estándar.
   @return La línea de la entrada estándar.
 */
char *lsh_read_line(void)
{
  int bufsize = RL_BUFFSIZE;
  int position = 0;
  //Asignamos un bloque de memoria.
  char *buffer = malloc(sizeof(char) * bufsize);
  int c;
  //Si no se puedo realizar la reserva de memoria, finalizamos ejecución.
  if (!buffer) {
    fprintf(stderr, "lsh: Error en la reserva de memoria\n");
    exit(EXIT_FAILURE);
  }

  while (1) {
	//c = getchar();

	// Lee caracter de entrada por teclado, almacenandolo como entero para comprobar EOF.
	c = fgetc(stdin);

	// Verifica si se preciono Ctrl+D
    if(feof(stdin))
    {
    	printf("\n");
    	exit (1);
    }

    // Si encontramos EOF, se reemplaza con un carácter nulo y retornamos el buffer.
    if (c == EOF || c == '\n') {
    	buffer[position] = '\0';
    	return buffer;
    	} else {
    		buffer[position] = c;
    	   	   }
    position++;

    // Si hemos superado el tamaño del búfer, reasignamos más espacio de memoria.
    if (position >= bufsize) {
      bufsize += RL_BUFFSIZE;
      buffer = realloc(buffer, bufsize);
      if (!buffer) {
        fprintf(stderr, "lsh: Error en la reserva de memoria\n");
        exit(EXIT_FAILURE);
      }
    }
  }
}

//----------------------------------------------------------------
/**
   @brief Dividir una cadena en token.
   @param la cadena
   @return Arreglo de tokens.
 */
char **lsh_split_line(char *line)
{
  int bufsize = TOK_BUFFSIZE, position = 0;
  char **tokens = malloc(bufsize * sizeof(char*));
  //char **operadores = malloc(bufsize * sizeof(char*));
  char *token, **tokens_backup;

  if (!tokens) {
    fprintf(stderr, "lsh: Error en la reserva de memoria\n");
    exit(EXIT_FAILURE);
  }

  token = strtok(line, TOK_DELIM);
  while (token != NULL) {
    tokens[position] = token;
    position++;

    if (position >= bufsize) {
      bufsize += TOK_BUFFSIZE;
      tokens_backup = tokens;
      tokens = realloc(tokens, bufsize * sizeof(char*));
      if (!tokens) {
		free(tokens_backup);
        fprintf(stderr, "lsh: Error en la reserva de memoria\n");
        exit(EXIT_FAILURE);
      }
    }

    token = strtok(NULL, TOK_DELIM);
  }
  tokens[position] = NULL;

  return tokens;
}

//----------------------------------------------------------------
/**
   @brief
   @param
   @return
 */
char **lsh_get_operadores(char **tokens)
{
	int bufsize = TOK_BUFFSIZE;
	char **operadores = malloc(bufsize * sizeof(char*));
	int j=0, pos=0;

	if (!operadores) {
	    fprintf(stderr, "lsh: Error en la reserva de memoria\n");
	    exit(EXIT_FAILURE);
	  }
	  /*while(tokens[i]!='\0')
		  {
		  printf("%s\n",tokens[i]);
		  i++;
		  }*/
	  while(tokens[j]!=NULL)
	  {
		  if (pos >= bufsize) {
		        bufsize += TOK_BUFFSIZE;
		        operadores = realloc(operadores, bufsize * sizeof(char*));
		        if (!operadores) {
		          fprintf(stderr, "lsh: Error en la reserva de memoria\n");
		          exit(EXIT_FAILURE);
		        }
		  }

		  if(!strcmp(tokens[j],">")||!strcmp(tokens[j],"<")||!strcmp(tokens[j],"|")||!strcmp(tokens[j],">>"))
		  {
			  //printf("Entre al IF\n");
			  operadores[pos]=tokens[j];
			  pos++;
			  //operadores++;
			  //printf("%s\n",*operadores);
		  }
		  j++;
	  }
	  operadores[pos]=NULL;

	  /*while(*operadores!='\0')
	  {
		  printf("Operador %s\n", *operadores);
		  operadores++;
	  }*/
	return operadores;
}

//----------------------------------------------------------------
/**
   @brief El loop ejecuta de forma secuencial los siguientes pasos:
   1)Leer el comando de entrada estandar,
   2)Separar la cadena de comandos en programa y argumentos,
   3)Ejecutar el comando analizado.
 */
void lsh_loop(void)
{
  char *line;
  char **args;
  char **operadores;
  int status,i=0;

  printf("\n");
  printf("ooooo  .oPYo.   .oPYo.            .oPYo.      .oo      .oo .oPYo.  o    o\n");
  printf("  8    8    8       `8            8   `8     .P 8     .P 8 8       8    8\n");
  printf("  8   o8YooP'      oP'           o8YooP'    .P  8    .P  8 `Yooo. o8oooo8\n");
  printf("  8    8        .oP'     ooooo    8   `b   oPooo8   oPooo8     `8  8    8\n");
  printf("  8    8        8'                8    8  .P    8  .P    8      8  8    8\n");
  printf("  8    8        8ooooo            8oooP' .P     8 .P     8 `YooP'  8    8\n\n");

  do {
    prompt();
    line = lsh_read_line();
    args = lsh_split_line(line);
    operadores = lsh_get_operadores(args);

    /*while(operadores[i]!=NULL)
    	  {
    		  printf("Operador %s\n", operadores[i]);
    		  i++;
    	  }*/

    status = lsh_execute(args, operadores);

    //Liberamos memoria
    free(line);
    free(args);
    free(operadores);
  } while (status);
}

//----------------------------------------------------------------
/**
   @brief
   @param argc Cantidad de argumentos.
   @param argv Vector de argumentos.
   @return
 */
int main(int argc, char **argv)
{
  // Poner aca archivos de configuración.

  //

  lsh_loop();

  // Tareas de apagado/limpieza aca

  //

  return EXIT_SUCCESS;
}
