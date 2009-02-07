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

  if (nprocs!=corregir_np(nprocs)){
    printf("Cantidad no valida de procesadores. Debe ser superior a 0 y potencia de 2 (1,2,4,8,...).\n");
    exit(-1);
  }

  MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
  if (myrank == ROOT){

    /*    
    // UTILIZADO PARA GENERAR VECTOR DE GRAN VOLUMEN DE MUESTRAS.
    // OMITE EL ARCHIVO DE POR MEDIO.    
    float f=770.0; n=1*32*32*1024; fs=10000; 
    senal = (scomplex*) malloc(n*sizeof(scomplex));          
    generar_coseno(senal,n,fs,f);
    */

    /*
    generar_archivo(); // Borrar esto.
    printf("ROOT.- Abriendo archivo...\n");            
    */

    if (argc>1){
      printf("* %u.- Leyendo entrada de %s.\n",myrank, argv[1]);
      abrir_archivo_t(argv[1], &senal, &n, &fs);     
    }else{
      printf("* %u.- Leyendo entrada de entrada.txt.\n",myrank);
      abrir_archivo_t("entrada.txt", &senal, &n, &fs);     
    }

    printf("* %u.- Iniciado el procesamiento de FFT (%d muestras).\n", myrank, n);
    tiempo = clock();  
    fft_res = (scomplex*) malloc(n*sizeof(scomplex));
  }

  fft(senal, fft_res, n,nprocs);            /* Procesar FFT. El vector senal y fft_res sólo se usan en ROOT.*/
  
  if (myrank == ROOT){
    tiempo = clock() - tiempo;
    printf("* %u.- Listo! FFT tardo %f segundos (CLOCKS_PER_SEC=%u).\n",myrank,(double)tiempo/(double)CLOCKS_PER_SEC,(uint)CLOCKS_PER_SEC);
    free(senal);    
    imprimir_maximo_modulo(fft_res, n, fs);
    
    if (argc>2){  
      printf("* %u.- Escribiendo FFT en %s.\n",myrank, argv[2]);
      escribir_archivo_f(argv[2], fft_res, n, fs);
    }

    if (argc>3){    
      printf("* %u.- Escribiendo |FFT| en %s.\n",myrank, argv[3]);
      escribir_archivo_mf(argv[3], fft_res, n, fs);
    }
    free(fft_res);
  }
    
  printf("* %u.- Finalizado.\n", myrank);
  MPI_Finalize();  
  
  return 0;
}

