#include <stdlib.h>
#include <stdio.h>
#include "archivo.h"
#include "complejo.h"
#include <time.h>
#define N 512*8*8*8*2

void escribir_fft_archivo_y_cerrar(FILE* fp, struct complex vector[], int n, float fs);

int main(){
  struct complex vector[N];
  int lx=N;
  long tiempo;
	float signi=1, scale=1;
  FILE* fp;
  FILE* fp2;
  FILE* fp3;
  
  float fs=1000.0;
  float f=300.0;
  
  fp  = (FILE*) abrir_archivo_out("senal_usada.txt");
  fp2 = (FILE*) abrir_archivo_out("salida.txt");
  fp3 = (FILE*) abrir_archivo_out("salidamod.txt");
  
  cargar_vector(vector,N,fs,f);
  escribir_archivo_y_cerrar(fp,vector, N);
  tiempo = clock();
  //imprimir_vector(vector,N);
	fft(lx, vector, signi, scale);
  //imprimir_vector(vector,N);
  tiempo = clock() - tiempo;
  escribir_fft_archivo_y_cerrar(fp2,vector, N,fs);
  escribir_modfft_archivo_y_cerrar(fp3,vector, N,fs);
  printf("Listo! Tardo %d ciclos.\n",tiempo);

  return 0;
}
