CC = gcc
FLAGS = -lm

# Makefile
gener : generador.o archivo.o complejo.o
	$(CC) $(FLAGS) -Wall generador.o archivo.o complejo.o -ogenerador
	rm -f *.o
	mv generador ./../generador

generador.o : generador.c
	$(CC) -c generador.c

archivo.o : ./../archivo.c
	$(CC) -c ./../archivo.c -oarchivo.o

complejo.o : ./../complejo.c
	$(CC) -c ./../complejo.c -ocomplejo.o

clean:
	rm -f *.o *~ ./../generador *.out

run:
	./../generador r 10000 333.333 1024
	mv entrada.txt ./../entrada.txt

all:
	make clean
	make
	make run

	
