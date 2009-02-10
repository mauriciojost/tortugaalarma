#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <mpi.h>
#include "archivo.h"
#include "complejo.h"
#include "fft.h"
#include "acc_fft.h"

#define ROOT 0
// Quitar comentarios en esta línea para veificar tiempos con n muy grandes.
//#define VECTOR_DIRECTAMENTE_EN_MEMORIA


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
    printf("\nModo de uso: %s <senal_entrada>.txt <fft_salida>.txt <modfft_salida>.txt\n\n", argv[0]);
#ifdef VECTOR_DIRECTAMENTE_EN_MEMORIA  
    // UTILIZADO PARA GENERAR VECTOR DE GRAN CANTIDAD DE MUESTRAS.
    // OMITE EL ARCHIVO DE POR MEDIO.    
    float f=770.7; n=16*1024*1024; fs=10000; 
    printf("* %u.- Cargando vector de muestras directamente en memoria...\n", myrank);
    senal = (scomplex*) malloc(n*sizeof(scomplex));          
    generar_coseno(senal,n,fs,f);
#else
    if (argc>1){
      printf("* %u.- Leyendo entrada de '%s'.\n",myrank, argv[1]);
      abrir_archivo_t(argv[1], &senal, &n, &fs);     
    }else{
      printf("* %u.- Leyendo entrada por defecto de 'entrada.txt'.\n",myrank);
      abrir_archivo_t("entrada.txt", &senal, &n, &fs);     
    }
#endif
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
    
#ifndef VECTOR_DIRECTAMENTE_EN_MEMORIA  
    if (argc>2){  
      printf("* %u.- Escribiendo FFT en '%s'.\n",myrank, argv[2]);
      escribir_archivo_f(argv[2], fft_res, n, fs);
    }else{
      printf("* %u.- Escribiendo FFT en 'fft.txt' por defecto.\n",myrank);
      escribir_archivo_f("fft.txt", fft_res, n, fs);
    }

    if (argc>3){    
      printf("* %u.- Escribiendo |FFT| en %s.\n",myrank, argv[3]);
      escribir_archivo_mf(argv[3], fft_res, n, fs);
    }
#endif

    free(fft_res);
  }
    
  printf("* %u.- Finalizado.\n", myrank);
  MPI_Finalize();  
  
  return 0;
}

