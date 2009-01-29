#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <mpi.h>
#include "archivo.h"
#include "complejo.h"
#include "fft.h"

#define ROOT 0
#define Ki 1024
#define N Ki*Ki*2


int main(int argc, char *argv[]){
  long n=N;
  float fs=1000.0, f=367.0;
  uint nprocs, myrank, i, tiempo;
  MPI_Status status;
  MPI_Datatype complex_MPI;
  scomplex *senal;
  scomplex *fft_res;
  
  senal = (scomplex*) malloc(n*sizeof(scomplex));
  fft_res = (scomplex*) malloc(n*sizeof(scomplex));

  MPI_Init(&argc, &argv);  

  MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
  MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
  
  if (myrank == ROOT){
    printf("ROOT.- Cargando vector...\n");
    cargar_vector_complejo(senal,n,fs,f);
    printf("ROOT.- Vector cargado.\n");
  }

  //printf("* Iniciado el procesamiento de FFT (%d muestras).\n", n);
  //tiempo = clock();
  fft(senal, fft_res, n,nprocs);            /* Procesar FFT. El vector X sólo se usa una vez.*/
  //tiempo = clock() - tiempo;
  free(senal);
  if (myrank == ROOT){
    imprimir_maximo_modulo(fft_res, n, fs);
  }
  //printf("Listo! FFT tardo %d ciclos.\n",tiempo);

  free(fft_res);
  printf("Yo (%u) digo chau!!!\n", myrank);
  MPI_Finalize();  
  
  return 0;
}

