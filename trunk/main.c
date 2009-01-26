#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <mpi.h>
#include "archivo.h"
#include "complejo.h"
#include "fft.h"

#define ROOT 0
#define Ki 1024
#define N Ki*Ki*32


int main(int argc, char *argv[]){
  
  long n=N;

  float fs=1000.0, f=367.0;
  int tiempo;

  unsigned int nprocs, myrank, i;
  MPI_Status status;
  MPI_Datatype complex_MPI;
  struct complex *senal;
  struct complex *fft_res;
  
  senal = (struct complex*) malloc(n*sizeof(struct complex));
  fft_res = (struct complex*) malloc(n*sizeof(struct complex));

  MPI_Init(&argc, &argv);  

  //MPI_Type_contiguous(2, MPI_DOUBLE, &complex_MPI);
  //MPI_Type_commit(&complex_MPI);

  MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
  MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
  
  if (myrank == ROOT){
    printf("Soy el ROOT. Cargando vector...\n");
    cargar_vector_complejo(senal,n,fs,f);
    printf("Soy el ROOT. Listo el vector. Enviando...\n");
    //MPI_Send(senal, n, complex_MPI, 1, 123, MPI_COMM_WORLD);
    printf("Soy el ROOT. Listo.\n");
  }//else{
    //printf("   Soy un hijo. Esperando vector...\n");
    //MPI_Recv(senal, n, complex_MPI, 0, 123, MPI_COMM_WORLD, &status);
    //printf("   Soy un hijo. Recibido.\n");
  

    /* AAA */    
    //printf("* Iniciado el procesamiento de FFT (%d muestras).\n", n);
    //tiempo = clock();
    ffttras(senal, fft_res, n,nprocs);            /* Procesar FFT. El vector X sólo se usa una vez.*/
    //tiempo = clock() - tiempo;
    //free(senal);
  if (myrank == ROOT){
    imprimir_maximo_modulo(fft_res, n, fs);
  }
    //printf("Listo! FFT tardo %d ciclos.\n",tiempo);
    /* AAA */    

  //}
  //free(fft_res);
  printf("Yo (%u) digo chau!!!\n", myrank);
  MPI_Finalize();  
  
  return 0;
}

