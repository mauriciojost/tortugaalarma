#include <stdlib.h>
#include <stdio.h>
#include "archivo.h"
#include "complejo.h"
#include <time.h>
#include <mpi.h>
#define K 1024
//#define N K*K*16
#define N 16

void escribir_fft_archivo_y_cerrar(FILE* fp, struct complex vector[], int n, float fs);

int main(int argc, char *argv[]){
  
  int n=N;
  long tiempo;
	float signi=1, scale=1;
  FILE *fp, *fp2, *fp3;
  float fs=1000.0, f=300.0;
  int men[4], nprocs, myrank, i;


  struct complex *vector;


  MPI_Init(&argc, &argv);  

  vector = (struct complex*) malloc(n*sizeof(struct complex));

  MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
  MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
  if (myrank == 0){
    for (i = 0; i < 4; i++)
      men[i] = i;
  }else{
    for (i = 0; i < 4; i++)
      men[i] = 0;
  }

  printf("Antes: %d %d %d %d\n", men[0], men[1],men[2], men[3]);
  MPI_Bcast(men, 4, MPI_INTEGER, 0, MPI_COMM_WORLD);
  printf("Despues: %d %d %d %d\n", men[0], men[1],men[2], men[3]);

 
  //fp  = (FILE*) abrir_archivo_out("senal_usada.txt");
  //fp2 = (FILE*) abrir_archivo_out("salida.txt");
  //fp3 = (FILE*) abrir_archivo_out("salidamod.txt");
  
  //cargar_vector_complejo(vector,n,fs,f);
  //escribir_archivo_y_cerrar(fp,vector, n);
  //tiempo = clock();
  //imprimir_vector(vector,n);
  //printf("* Iniciado el procesamiento de FFT (%d muestras).\n", n);
	//fft(n, vector, signi, scale);
  //printf("* Procesamiento de FFT terminado.\n");
  //imprimir_vector(vector,N);
  //tiempo = clock() - tiempo;
  //imprimir_maximo_modulo(vector, n, fs);
  //escribir_fft_archivo_y_cerrar(fp2,vector, n,fs);
  //escribir_modfft_archivo_y_cerrar(fp3,vector, n,fs);
  //printf("Listo! FFT tardo %d ciclos.\n",tiempo);
  MPI_Finalize();
  return 0;
}


























