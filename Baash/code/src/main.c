/*
 * main.c
 *
 *      Author: Ceballos, Matias
 *      		Zimmel, Ezequiel
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <string.h>
#include "../include/prompt.h"
#include "../include/path.h"
#include "../include/manipulacion_archivos.h"
#include "../include/builtin.h"

int background(char* argv[]);


int main()
{
	int pid, statusPID, bgProcess, pipeExecuted; //Banderas de control
	int argC;
	char* argV[20];
	char* argv1[20]; //Para el caso que se haga un pipeline.
	char* argv2[20];
	char executePath[256]; //Path del proceso que se va a ejecutar.
	char comando [256]; //Comando que ingresa el usuario.

	int pathCounter;
	char *paths[20];
	//Obtengo todos los paths que están en la variable de entorno PATH; y su cantidad.
	pathCounter = get_path_entries(paths);

	 printf("\n");
	 printf("ooooo  .oPYo.   .oPYo.            .oPYo.      .oo      .oo .oPYo.  o    o\n");
	 printf("  8    8    8       `8            8   `8     .P 8     .P 8 8       8    8\n");
	 printf("  8   o8YooP'      oP'           o8YooP'    .P  8    .P  8 `Yooo. o8oooo8\n");
	 printf("  8    8        .oP'     ooooo    8   `b   oPooo8   oPooo8     `8  8    8\n");
	 printf("  8    8        8'                8    8  .P    8  .P    8      8  8    8\n");
	 printf("  8    8        8ooooo            8oooP' .P     8 .P     8 `YooP'  8    8\n\n");

	int status=1, buil;
	while(status){
		buil=0;

		statusPID = 0;	   //Limpio las banderas
		pipeExecuted = 0;

		prompt();
		fgets(comando,256,stdin);

		//Si comando tiene solo un salto de línea, no hago nada.
		if(!strcmp(comando, "\n")){
			//printf("\n");
			status=1;
		}

		//Esto es lo que se ejecuta si se ingresa algun comando
		else{
			argC = read_input(argV, comando);

			int i=0;
			//Comandos internos del bash.
			for(i=0;i < lsh_num_builtins();i++)
			  	  {
				      if (strcmp(argV[0], builtin_str[i]) == 0) {
				        buil = (*builtin_func[i])(argV);
				        if(buil==0)
				        	exit(0);
				      }
			  	  }

			int flagRedirect = 0;
			int doPipe = 0;
			char fileName[50];

			//Verifica si se debe ejecutar un pipe
			doPipe = check_Pipe(argV, argv1, argv2);
			flagRedirect = check_Redirect(argV, fileName);

			//Verifico si el proceso se tiene que ejecutar en Background.
			bgProcess = background(argV);
			if(bgProcess){
				argV[argC-1] = NULL;
				argC--;
			}

			search_File_Dir(argV[0], paths, executePath);  //Arma el directorio del ejecutable
			if(executePath[0] == 'X' && status==0)
				printf("El archivo no fue encontrado\n");
			else{
				pid = fork();
				if (pid<0) {
					perror("Creando el hijo");
					exit(1);
				}
				else if (pid == 0) {	//Proceso hijo.
					if(flagRedirect == 2){
						status = out_Redirect(fileName);
					}
					else if(flagRedirect == 1){
						status = in_Redirect(fileName);
						if(status!=0){
							close(status);
							status=1;
						}
						//freopen(fileName,"r",stdin);
					}
					else if(doPipe == 1){
						exec_Pipeline(argv1, argv2, paths);
						pipeExecuted = 1;
					}
					if(!pipeExecuted){
						if(execv(executePath, argV)== -1 && buil==0)
						perror("Error");
						exit(1);
					}
				}
				else{	//Proces padre.
					statusPID = -1;
				}
				if(bgProcess)
					waitpid(pid,&statusPID,WNOHANG);
				else
					waitpid(pid,&statusPID,0);
			}
		}
	}

	return 0;
}

//----------------------------------------------------------------
/**
 * @brief Verifica si se quiere lanzar el proceso en background.
 * @param argv Vector de argumentos que pasa el usuario. Se necesita para verificar si el ultimo es un &.
 * @return Devuelve 0 si el proceso no debe ejecutarse en background, y 1 en caso contrario.
 */
int background(char* argv[]){
	int i;
	for(i = 0; i < 20; i++){
		if(argv[i] == NULL)
			break;
	}
	if(!strcmp(argv[i-1], "&"))
		return 1;
	return 0;
}


