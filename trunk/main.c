#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <mpi.h>
#include "archivo.h"
#include "complejo.h"
#include "fft.h"

#define ROOT 0
#define Ki 1024
#define N Ki*Ki*8


int main(int argc, char *argv[]){
  
  long n=N;
  FILE *fp3, *fp2, *fp;
  float fs=1000.0, f=187.9;
  int tiempo;
  unsigned int np = 8;

  int nprocs, myrank, i;
  MPI_Status status;
  MPI_Datatype complex_MPI;
  MPI_Datatype vector_complex_MPI;
  struct complex *senal;

  

  senal = (struct complex*) malloc(n*sizeof(struct complex));
  struct complex *fft_res;
  fft_res = (struct complex*) malloc(n*sizeof(struct complex));

  MPI_Init(&argc, &argv);  

  MPI_Type_contiguous(2*n, MPI_DOUBLE, &vector_complex_MPI);
  MPI_Type_commit(&vector_complex_MPI);

  MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
  MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
    

  
  
  



  
  if (myrank == ROOT){
    printf("Soy el ROOT. Cargando vector...\n");
    cargar_vector_complejo(senal,n,fs,f);
    printf("Soy el ROOT. Listo el vector. Enviando...\n");
    MPI_Send(senal, 1, vector_complex_MPI, 1, 123, MPI_COMM_WORLD);
    //MPI_Bcast(senal, 1, vector_complex_MPI, 0, MPI_COMM_WORLD);
    printf("Soy el ROOT. Listo.\n");
  }else{
    printf("   Soy un hijo. Esperando vector...\n");
    MPI_Recv(senal, 1, vector_complex_MPI, 0, 123, MPI_COMM_WORLD, &status);
    //MPI_Bcast(senal, 1, vector_complex_MPI, 0, MPI_COMM_WORLD);
    printf("   Soy un hijo. Recibido.\n");
  




    //fp  = (FILE*) abrir_archivo_out("senal_usada.txt");
    //fp2 = (FILE*) abrir_archivo_out("salida.txt");
    //fp3 = (FILE*) abrir_archivo_out("salidamod.txt");
    
    //escribir_archivo_y_cerrar(fp,vector, n);
    printf("* Iniciado el procesamiento de FFT (%d muestras).\n", n);
    tiempo = clock();
    ffttras(senal, fft_res, n,np);            /* Procesar FFT. El vector X sólo se usa una vez.*/
    tiempo = clock() - tiempo;
    free(senal);
    //printf("* Procesamiento de FFT terminado.\n");
    imprimir_maximo_modulo(fft_res, n, fs);
    //escribir_modfft_archivo_y_cerrar(fp3,vector2, n,fs);
    printf("Listo! FFT tardo %d ciclos.\n",tiempo);
  }
  MPI_Finalize();  
  free(fft_res);
  return 0;
}

