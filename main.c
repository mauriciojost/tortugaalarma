#include <stdlib.h>
#include <stdio.h>
#include "archivo.h"
#include "complejo.h"
#include "fft.h"
#include <time.h>

#define Ki 1024
#define N Ki*Ki*16


int main(int argc, char *argv[]){
  
  long n=N;
  FILE *fp3, *fp2, *fp;
  float fs=1000.0, f=77.0;
  int tiempo;
  struct complex *vector;
  vector = (struct complex*) malloc(n*sizeof(struct complex));
  struct complex *vector2;
  vector2 = (struct complex*) malloc(n*sizeof(struct complex));

  
  cargar_vector_complejo(vector,n,fs,f);

  //fp  = (FILE*) abrir_archivo_out("senal_usada.txt");
  //fp2 = (FILE*) abrir_archivo_out("salida.txt");
  //fp3 = (FILE*) abrir_archivo_out("salidamod.txt");
  
  
  //escribir_archivo_y_cerrar(fp,vector, n);
  printf("* Iniciado el procesamiento de FFT (%d muestras).\n", n);
  tiempo = clock();
  ffttras(vector, vector2, n);            /* Procesar FFT. El vector X sólo se usa una vez.*/
  tiempo = clock() - tiempo;
  printf("* Procesamiento de FFT terminado.\n");
  imprimir_maximo_modulo(vector2, n, fs);
  //escribir_modfft_archivo_y_cerrar(fp3,vector2, n,fs);
  printf("Listo! FFT tardo %d ciclos.\n",tiempo);
  free(vector);
  free(vector2);
  return 0;
}

