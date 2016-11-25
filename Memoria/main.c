#include <stdio.h>
#include "malloc.h"

void alocar_memoria(void *ptr[]);
void liberar_memoria(void *ptr[]);

int i=0;

/* Metodo main() para testeo de las funciones */
int main(int argc, char *argv[]){

  void *ptr[25]={NULL};
  int valor=0;

  printf("\nImplementación de malloc() y free():\n");
  printf("--------------------------------------------\n");

  while(valor!=9)
  {
  printf("OPCIONES: Alocar  [0]\n"
		  "	  Liberar [1]\n"
		  "	  Salir   [9]\n");
  printf("Opcion: ");
  scanf("%d",&valor);
  switch(valor) {
        case 0 :
        	printf("\n");
        	if(*ptr!=NULL)
        	actualizar_indices(ptr);
        	alocar_memoria(ptr);
        	actualizar_indices(ptr);
        	printf("Bloques de memoria alocada:\n");
        	imprimir_bloques_memoria(ptr);
        	printf("\n");
           break;
        case 1 :
        	if(*ptr==NULL)
        	{
        		printf("ERROR, no hay memoria alocada\n");
        		break;
        	}
        	printf("\n");
        	printf("Libero bloques con free():\n");
            actualizar_indices(ptr);
            liberar_memoria(ptr);
        	actualizar_indices(ptr);
            printf("Bloques de memoria alocada:\n");
            imprimir_bloques_memoria(ptr);
            printf("\n");
           break;
        case 9 :
        	return 0;
        default :
           printf("Valor invalido\n" );
     }
  }
	return 0;
}

void alocar_memoria(void *ptr[])
{
	int valor;
	int continuar=1;
	while(continuar){
	  printf("Ingrese el tamaño de memoria a alocar: ");
	  scanf("%d",&valor); //Leyendo y almacenando el número solicitado
	  ptr[i] = malloc(valor);
	  i++;
	  printf("Desea alocar otro bloque de memoria? Si[1] No[0]: ");
	  scanf("%d",&continuar);
	  }
}

void liberar_memoria(void *ptr[])
{
	int valor;
    int continuar=1;
	while(continuar){
	  printf("Ingrese el N° Bloque a liberar: ");
	  scanf("%d",&valor); //Leyendo y almacenando el número solicitado
	  free(ptr[valor]);
	  printf("Desea liberar otro bloque de memoria? Si[1] No[0]: ");
	  scanf("%d",&continuar);
	  }
}
