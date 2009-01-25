CC = mpicc
FLAGS = -lm

# Makefile
fft : main.o fft.o complejo.o archivo.o
	$(CC) $(FLAGS) -Wall main.o fft.o complejo.o archivo.o -offt
	rm -f *.o

main.o : main.c
	$(CC) -c main.c

fft.o : fft.c
	$(CC) -c fft.c

complejo.o : complejo.c
	$(CC) -c complejo.c

archivo.o : archivo.c
	$(CC) -c archivo.c

clean: 
	rm -f *.o *~ fft *.out *.txt

all:
	make clean
	make
	echo "**************************************************"
	mpirun -np 4 ./fft
