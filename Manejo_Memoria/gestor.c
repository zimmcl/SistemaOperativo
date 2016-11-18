/*
 * gestor.c
 *
 *  Created on: 17/11/2016
 *      Author: ezequiel
 */

#include <stdio.h>
#include <unistd.h>
#include "gestor.h"

/* El programa busca un bloque memoria libre lo suficientemente grande para guardar el bloque
 * solicitado. El bloque libre es dividido si es necesario y se marca como free.
 * Al liberar un bloque, en caso de estar libres los bloques anterior o siguiente, se unen.
 * */

/* Tamaño que ocupa la estructura del bloque */
#define BLOCK_SIZE sizeof(struct s_block)

/* Para alinear punteros, se utilizan tamaños muliplos de 32 bits (4 bytes) */
#define alinear4(x) (((((x)-1)>>2)<<2)+4)


/* Inicio de la lista doblemente enlazada */
void *base = NULL;

struct s_block *b;

/* Función malloc() */
void *malloc(size_t size) {
	struct s_block *block, *last;

  /* Alineo el tamaño requerido al múltiplo de 4 más cercano */
  size = alinear4(size);

  /* Controlo que el tamaño sea positivo */
  if (size <= 0) {
    return NULL;
  }

  if (!base) {
    /* Si base es NULL, es la primera vez que se llama a malloc() */
    /* Como es la primera vez, llamo a extender_heap */
    block = extender_heap(NULL, size);
    if (!block) {
      /* Si ocurrio algún error, salgo */
      return NULL;
    }
    /* Coloco al nuevo bloque como la base de la lista */
    base = block;
  } else {
    /* Si no es la primera vez que se llama malloc() */

    /* Veo si hay un bloque libre del tamaño requerido o mayor */
    /* Para esto, recorro la lista desde el principio (First Fit) */
    last = base;
    block = encontrar_bloque_libre(&last, size);
    if (!block) {
      /* Si no se encontro nigún bloque, llamo a extender_heap */
      block = extender_heap(last, size);
      if (!block) {
        /* Si hubo un error, salgo de la función */
        return NULL;
      }
    } else {
        /* Si se encontro un bloque libre, me fijo que sea del tamaño necesario */
        if((block->size - size) >= BLOCK_SIZE + 4) {
          /* Y divido, tomo el espacio que necesito y armo otro bloque con el resto */
          dividir_bloque(block, size);
        }

        /* Pongo el bloque que estaba libre, como ocupado */
        block->free = 0;
    }
  }

  //actualizar_indices(base);
  return(block + 1);
}

/* Función free() */
void free(void *ptr) {
  /* Controlo que el puntero pasado no sea NULL */
  if (!ptr) {
    return;
  }

  /* Obtengo el puntero al bloque apuntado por "ptr" */
  struct s_block *block = obtener_puntero_bloque(ptr);

  /* Marco el bloque como libre */
  block->free = 1;

  /* Veo si hay bloques libres antes o después, intento fusionarlos */
  if(block->prev && block->prev->free) {
    block = fusionar_bloques(block->prev);
  }

  if(block->next) {
    block = fusionar_bloques(block);
  } else {
    /* Se llego al final de la lista */
    if (block->prev) {
      /* Pongo al último bloque como libre */
      block->free = 1;
    }
  }
  //actualizar_indices(ptr);
}

/* Función dividir_bloque() */
void dividir_bloque(struct s_block *block, size_t size) {
  /*Guardo el tamaño total del bloque actual*/
  size_t total_size = block->size;

  /* Creo un bloque nuevo, que comience despues que
     termine la primera parte del bloque dividido */
  struct s_block *nuevo = (struct s_block *) block->data + size;

  /* Si el bloque completo tenia un siguiente */
  if(block->next) {
    /* Pongo el siguiente del nuevo, como el siguiente del anterior */
    nuevo->next = block->next;

    /* Y el anterior del siguiente del nuevo como el nuevo */
    nuevo->next->prev = nuevo;
  }

  /* El nuevo tamaño del bloque "block" es size */
  block->size = size;
  /* Y el siguiente bloque es el "nuevo" */
  block->next = nuevo;

  /* El tamaño del nuevo es el tamaño total del anterior menos el tamaño
     del bloque, y el tamaño de la memoria utilizada en la primera division */
  nuevo->size = total_size - size - BLOCK_SIZE;

  /* Pongo al anterior del nuevo, como el original */
  nuevo->prev = block;
  /* Y lo marco como libre */
  nuevo->free = 1;
}

/* Función fusionar_bloques() */
struct s_block *fusionar_bloques (struct s_block *block) {
  /* Me fijo si el bloque tiene un siguiente y está libre*/
  if (block->next && block->next ->free ){
    /* Fusiono ambos bloques */
    block->size += BLOCK_SIZE + block->next ->size;
    block->next = block->next->next;
    if (block->next) {
      /* Pongo al anterior del siguiente como el actual */
      block->next->prev = block;
    }
  }

  return (block);
}

/* Función encontrar_bloque_libre() */
struct s_block *encontrar_bloque_libre(struct s_block **last, size_t size) {
  /* Recorro la lista desde la base buscando un bloque que este libre
     y que su tamaño sea mayor o igual al tamaño necesitado  */
	struct s_block *current = base;
  while (current && !(current->free && current->size >= size)) {
    *last = current;
    current = current->next;
  }

  /* Retorno un bloque si lo encontré o NULL */
  return current;
}

/* Función extender_heap() */
struct s_block *extender_heap(struct s_block *last, size_t size) {
  /* Creo un nuevo bloque */
	struct s_block *block;

  /* Obtengo el break y coloco el bloque ahí */
  block = sbrk(0);
  /* Intento aumentar el break en el tamaño requerido,
     mas el tamaño del bloque */
  void *request = sbrk(size + BLOCK_SIZE);

  /* Si no se pudo aumentar el break, retorno NULL, hubo un error */
  if (request == (void*) -1) {
    return NULL;
  }

  /* Si no es la primera vez, actualizo el next del último */
  if (last) {
    last->next = block;
    block->index = last->index+1;
  }

  /* Actualizo el tamaño, el siguiente, el anterior y el estado (free) del nuevo bloque */
  block->size = size;
  block->next = NULL;
  block->prev = last;
  block->free = 0;

  /* Retorno el bloque recientemente alocado */
  return block;
}

/* Función obtener_puntero_bloque() */
struct s_block *obtener_puntero_bloque(void *ptr) {
  /* Retorno el puntero al bloque deseado */
  return (struct s_block *) ptr-1;
}


void actualizar_indices(void *ptr){
    printf("A: valor puntero %p: \n", ptr);
	obtener_puntero_bloque(ptr)->index=0;
	printf("Valor indice: %d \n", obtener_puntero_bloque(ptr)->index);
	while(obtener_puntero_bloque(ptr)->next!=NULL)
	{
		printf("B: valor puntero %p: \n", ptr);
		obtener_puntero_bloque(ptr)->next->index=(obtener_puntero_bloque(ptr)->index)+1;
		ptr++;
		printf("Valor indice: %d \n", obtener_puntero_bloque(ptr)->index);
		printf("sume ptr\n");
	}
	/*Actualizo los indices de los bloques*/
	/*b = obtener_puntero_bloque(ptr);
	b->index=0;
	while(b->next != NULL) {
	      	b->next->index=b->index+1;
	       	b = b->next;
	  	}*/
	printf("B: salgo\n");
}

/* Función imprimir_lista_memoria() */
/* Sólo para debugging, imprime la lista doblemente enlazada
   de una forma que sea sencilla de ver e interpretar */
void imprimir_lista_memoria(void *ptr) {
	b = obtener_puntero_bloque(ptr);

	/* Si la lista esta vacia no hago nada */
	if(b == NULL) return;

	/* Si no esta vacia, la recorro e imprimo los datos en forma de tabla */

	printf("+----------+----------+----------+------------+------------+------------+\n");
	printf("| %8s | %8s | %8s | %10s | %10s | %10s |\n", "Nº Bloque", "Tamaño", "Libre", "Anterior", "Actual", "Siguiente");
	printf("+----------+----------+----------+------------+------------+------------+\n");

	do {
		printf("| %8d | %8zu | %8s | %10p | %10p | %10p |\n", b->index, b->size, (b->free) ? "Si": "No", b->prev, b, b->next);
		b=b->next;
	} while(b != NULL);

	printf("+----------+----------+----------+------------+------------+------------+\n");

}
