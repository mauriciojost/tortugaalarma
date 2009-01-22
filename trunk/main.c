#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "archivo.h"
#include "complejo.h"
#include "fft.h"


#define Ki 1024
#define N Ki*Ki


int main(int argc, char *argv[]){
  
  long n=N;
  FILE *fp3, *fp2, *fp;
  float fs=1000.0, f=176.5;
  int tiempo;
  struct complex *senal;

  senal = (struct complex*) malloc(n*sizeof(struct complex));
  struct complex *fft_res;
  fft_res = (struct complex*) malloc(n*sizeof(struct complex));

  
  cargar_vector_complejo(senal,n,fs,f);

  //fp  = (FILE*) abrir_archivo_out("senal_usada.txt");
  //fp2 = (FILE*) abrir_archivo_out("salida.txt");
  //fp3 = (FILE*) abrir_archivo_out("salidamod.txt");
  
  
  //escribir_archivo_y_cerrar(fp,vector, n);
  printf("* Iniciado el procesamiento de FFT (%d muestras).\n", n);
  tiempo = clock();
  ffttras(senal, fft_res, n);            /* Procesar FFT. El vector X sólo se usa una vez.*/
  tiempo = clock() - tiempo;
  free(senal);
  printf("* Procesamiento de FFT terminado.\n");
  imprimir_maximo_modulo(fft_res, n, fs);
  //escribir_modfft_archivo_y_cerrar(fp3,vector2, n,fs);
  printf("Listo! FFT tardo %d ciclos.\n",tiempo);
  
  free(fft_res);
  return 0;
}

