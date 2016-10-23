.DELETE_ON_ERROR:#Borra el target en caso de producirse un error en alguno de los comandos

#Definimos las macros
CC=gcc	#Compilador a usar
CFLAGS= -Wall -pedantic -g	#Banderas a utilizar
OBJS= main.o manipulacion_archivos.o builtin.o prompt.o path.o #Se pueden incluir mas ficheros a procesar
vpath %.h include
vpath %.c src

baash: ${OBJS}
	${CC} ${CFLAGS} -o baash ${OBJS}
	@echo "Compilación exitosa | ${shell date --iso=seconds}"

main.o: main.c manipulacion_archivos.h builtin.h prompt.h path.h
	${CC} -c $< -o $@

builtin.o: builtin.c builtin.h prompt.h
	${CC} -c $< -o $@

prompt.o: prompt.c builtin.h manipulacion_archivos.h
	${CC} -c $< -o $@

manipulacion_archivos.o: manipulacion_archivos.c manipulacion_archivos.h
	${CC} -c $< -o $@

path.o: path.c path.h
	${CC} -c $< -o $@
    
clean_o:
	rm -f ${OBJS}
	@echo "Se eliminaron correctamente archivos *.o"

clean_all:
	@rm -f ${OBJS} baash
	@echo "Se eliminaron correctamente todos los archivos."
