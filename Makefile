CC = mpicc
FLAGS = -lm

# Makefile
fft : main.o fft.o complejo.o archivo.o acc_fft.o
	$(CC) $(FLAGS) -Wall main.o fft.o complejo.o archivo.o acc_fft.o -offt
	rm -f *.o

main.o : main.c
	$(CC) -c main.c

fft.o : fft.c
	$(CC) -c fft.c

complejo.o : complejo.c
	$(CC) -c complejo.c

archivo.o : archivo.c
	$(CC) -c archivo.c

acc_fft.o : acc_fft.c
	$(CC) -c acc_fft.c

clean:
	rm -f *.o *~ fft *.out

cleana:
	rm -f *.o *~ fft *.out *.txt gener

run:
	echo "mpirun -np 2 ./fft"
	./fft entrada.txt fft.txt fftm.txt

gener: ./gen/generador.c
	gcc ./gen/generador.c -ogener

all:
	make clean
	make
	make run
	echo "**************************************************"
	
