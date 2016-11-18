.DELETE_ON_ERROR:#Borra el target en caso de producirse un error en alguno de los comandos

#Definimos las macros
CC=gcc	#Compilador a usar
CFLAGS= -Wall -pedantic	#Banderas a utilizar
OBJS= main.o gestor.o #Se pueden incluir mas ficheros a procesar
#vpath %.h include
#vpath %.c src

Memoria: ${OBJS}
	${CC} ${CFLAGS} -o Memoria ${OBJS}
	@echo "Compilación exitosa | ${shell date --iso=seconds}"

main.o: main.c gestor.h
	${CC} -c $< -o $@

gestor.o: gestor.c gestor.h
	${CC} -c $< -o $@
    
clean_o:
	@rm -f ${OBJS}
	@echo "Se eliminaron correctamente archivos *.o"

clean_all:
	@rm -f ${OBJS} Memoria
	@echo "Se eliminaron correctamente todos los archivos."
