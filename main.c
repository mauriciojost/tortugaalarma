#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <mpi.h>
#include "archivo.h"
#include "complejo.h"
#include "fft.h"
#include "acc_fft.h"

#define ROOT 0

int main(int argc, char *argv[]){
  float fs;
  uint nprocs, n, myrank, tiempo;
  MPI_Status status;
  MPI_Datatype complex_MPI;
  scomplex *senal, *fft_res;

  MPI_Init(&argc, &argv);  
  MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

  if (nprocs!=corregir_np(nprocs) || (nprocs<1)){
    printf("Cantidad no valida de procesadores. Debe ser superior a 0 y potencia de 2 (1,2,4,8,...).\n");
    exit(-1);
  }

  MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
  if (myrank == ROOT){
    
    float f=770.0; n=1*32*32*1024; fs=10000; 
    senal = (scomplex*) malloc(n*sizeof(scomplex));          
    generar_coseno(senal,n,fs,f);
    
    /*
    generar_archivo(); // Borrar esto (para la entrega)*********.
    printf("ROOT.- Abriendo archivo...\n");        
    abrir_archivo_t("entrada.txt", &senal, &n, &fs);
    */

    printf("ROOT.- Iniciado el procesamiento de FFT (%d muestras).\n", n);
    tiempo = clock();  
    fft_res = (scomplex*) malloc(n*sizeof(scomplex));
  }

  fft(senal, fft_res, n,nprocs);            /* Procesar FFT. El vector senal y fft_res sólo se usan en ROOT.*/
  
  if (myrank == ROOT){
    tiempo = clock() - tiempo;
    printf("ROOT.- Listo! FFT tardo %f segundos (CLOCKS_PER_SEC=%u).\n",(double)tiempo/(double)CLOCKS_PER_SEC,(uint)CLOCKS_PER_SEC);
    free(senal);    
    imprimir_maximo_modulo(fft_res, n, fs);
    //escribir_archivo_f("fft.txt", fft_res, n, fs);
    escribir_archivo_mf("fftm.txt", fft_res, n, fs);
    free(fft_res);
  }
    
  printf("* %u.- Finalizado.\n", myrank);
  MPI_Finalize();  
  
  return 0;
}

