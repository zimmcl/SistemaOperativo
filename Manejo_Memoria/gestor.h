/*
 * gestor.h
 *
 *  Created on: 17/11/2016
 *      Author: ezequiel
 */

#ifndef GESTOR_H_
#define GESTOR_H_

//typedef struct s_block;

/* Estructura del bloque */
struct s_block {
  int index;				//Identificador
  size_t size;				//Tamaño
  struct s_block *prev;		//Enlaza bloque previo
  struct s_block *next;		//Enlaza bloque siguiente
  int free;					//Bloque libre
  char data[1];				/*Char que ocupa toda la memoria asignada. Aca se escribirá lo que
  	  	  	  	  	  	  	  se quiera guardar en la memoria */
};

/* Declaración de la función malloc()
 * @Param size: tamaño que queremos de memoria */
void *malloc(size_t size);

/* Declaración de la función free() */
void free(void *ptr);

/* Declaración de funciones auxiliares */
struct s_block *encontrar_bloque_libre(struct s_block **last, size_t size);
struct s_block *extender_heap(struct s_block *last, size_t size);
struct s_block *obtener_puntero_bloque(void *ptr);
struct s_block *fusionar_bloques (struct s_block *block);
void dividir_bloque(struct s_block *block, size_t size);
void actualizar_indices(void *ptr);
void imprimir_lista_memoria(void *ptr);

#endif /* GESTOR_H_ */
