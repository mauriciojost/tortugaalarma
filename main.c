#include <stdlib.h>
#include <stdio.h>
#include "archivo.h"
#include "complejo.h"
#include <time.h>
#define K 1024
#define N K*K*16*2


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
  
  cargar_vector_complejo(vector,n,fs,f);

  ffttras();            /* Procesar FFT. El vector X sólo se usa una vez.*/

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



  return 0;
}






