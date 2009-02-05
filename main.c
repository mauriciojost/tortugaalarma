#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <mpi.h>
#include "archivo.h"
#include "complejo.h"
#include "fft.h"
#include "acc_fft.h"

#define ROOT 0
#define Ki 1024
#define MiB Ki*Ki/16 // 16 MB son logrados con 1M muestras (1 cmplx -> 2 doubles -> 16 bytes).
#define N 128*32//MiB/2

void generar_archivo(){
  float fs=10000.0, f=1234.56789012345;
  scomplex *senial;
  printf("*** Cargando vector...\n");
  senial = (scomplex*) malloc(N*sizeof(scomplex));          
  cargar_vector_complejo(senial,N,fs,f);
  printf("*** Guardando archivo...\n");
  escribir_archivo_t("entrada.txt", senial, N, fs);
  free(senial);
}



int main(int argc, char *argv[]){
  uint n=N;
  float fs;
  uint nprocs, myrank, i, tiempo;
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
    generar_archivo();

    printf("ROOT.- Abriendo archivo...\n");        
    abrir_archivo_t("entrada.txt", &senal, &n, &fs);
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

    escribir_archivo_f("fft.txt", fft_res, n, fs);
        
    free(fft_res);
  }
    
  printf("* %u.- Finalizado.\n", myrank);
  MPI_Finalize();  
  
  return 0;
}

