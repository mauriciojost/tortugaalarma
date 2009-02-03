#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <mpi.h>
#include "archivo.h"
#include "complejo.h"
#include "fft.h"

#define ROOT 0
#define Ki 1024
#define MiB Ki*Ki/16 // 16 MB son logrados con 1M muestras (1 cmplx -> 2 doubles -> 16 bytes).
#define N MiB*16

int main(int argc, char *argv[]){
  long n=N;
  float fs=10000.0, f=1234.56789012345;
  uint nprocs, myrank, i, tiempo;
  MPI_Status status;
  MPI_Datatype complex_MPI;
  scomplex *senal;
  scomplex *fft_res;
  
  
  MPI_Init(&argc, &argv);  



  MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
  MPI_Comm_size(MPI_COMM_WORLD, &nprocs);


  if (myrank == ROOT){
    printf("ROOT.- Cargando vector...\n");
    senal = (scomplex*) malloc(n*sizeof(scomplex));          
    cargar_vector_complejo(senal,n,fs,f);
    printf("ROOT.- Vector cargado.\n");
    fft_res = (scomplex*) malloc(n*sizeof(scomplex));
  }

  //printf("Detenido (%d).\n",myrank);getchar();




  //printf("* Iniciado el procesamiento de FFT (%d muestras).\n", n);
  //tiempo = clock();
  fft(senal, fft_res, n,nprocs);            /* Procesar FFT. El vector senal y fft_res sólo se usan en ROOT.*/
  //tiempo = clock() - tiempo;


  if (myrank == ROOT){
    free(senal);    
    imprimir_maximo_modulo(fft_res, n, fs);
    free(fft_res);
  }
  //printf("Listo! FFT tardo %d ciclos.\n",tiempo);

  
  printf("* %u.- Finalizado.\n", myrank);
  MPI_Finalize();  
  
  return 0;
}

