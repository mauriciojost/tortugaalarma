#include <stdlib.h>
#include <stdio.h>
#include "archivo.h"
#include "complejo.h"
#include <time.h>
#include <mpi.h>
#define K 1024
#define N K*K*16*2
#define ROOT 0

void escribir_fft_archivo_y_cerrar(FILE* fp, struct complex vector[], int n, float fs);

int main(int argc, char *argv[]){
  
  long n=N;
  long tiempo;
	float signi=1, scale=1;
  FILE *fp, *fp2, *fp3;
  float fs=1000.0, f=300.0;
  int men[4], nprocs, myrank, i;
  MPI_Status status;
  MPI_Datatype complex_MPI;
  MPI_Datatype vector_complex_MPI;

  struct complex *vector;
  vector = (struct complex*) malloc(n*sizeof(struct complex));
  

  
  



  MPI_Init(&argc, &argv);  



  //MPI_Type_contiguous(2, MPI_DOUBLE, &complex_MPI);
  //MPI_Type_commit(&complex_MPI);
  //MPI_Type_contiguous(n, complex_MPI, &vector_complex_MPI);
  //MPI_Type_commit(&vector_complex_MPI);
  MPI_Type_contiguous(2*n, MPI_DOUBLE, &vector_complex_MPI);
  MPI_Type_commit(&vector_complex_MPI);

  MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
  MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
  

  if (myrank == ROOT){
    printf("Soy el ROOT. Cargando vector...\n");
    cargar_vector_complejo(vector,n,fs,f);
    printf("Soy el ROOT. Listo el vector. Enviando...\n");
    //MPI_Send(vector, 1, vector_complex_MPI, 1, 123, MPI_COMM_WORLD);
    MPI_Bcast(vector, 1, vector_complex_MPI, 0, MPI_COMM_WORLD);
    printf("Soy el ROOT. Listo.\n");
  }else{
    printf("   Soy un hijo. Esperando vector...\n");
    //MPI_Recv(vector, 1, vector_complex_MPI, 0, 123, MPI_COMM_WORLD, &status);
    MPI_Bcast(vector, 1, vector_complex_MPI, 0, MPI_COMM_WORLD);
    printf("   Soy un hijo. Recibido.\n");
  }



/*

  printf("Antes: %d %d %d %d\n", men[0], men[1],men[2], men[3]);
  
  printf("Despues: %d %d %d %d\n", men[0], men[1],men[2], men[3]);

 */

  //fp  = (FILE*) abrir_archivo_out("senal_usada.txt");
  //fp2 = (FILE*) abrir_archivo_out("salida.txt");
  //fp3 = (FILE*) abrir_archivo_out("salidamod.txt");
  
  
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






