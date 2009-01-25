#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "archivo.h"
#include "complejo.h"

FILE* abrir_archivo_in(char *nombre){
  FILE *fp;
  fp = fopen (nombre, "r");
  if ((int)fp==-1){
    printf("Error al abrir el archivo '%s'.\n",nombre);
    exit(-1);
  }
  return fp;
}

FILE* abrir_archivo_out(char *nombre){
  FILE* fp;
  fp = fopen (nombre, "w");
  if ((int)fp==-1){
    printf("Error al abrir el archivo '%s'.\n",nombre);
    exit(-1);
  }else{
    printf("Archivo '%s' abierto correctamente.\n",nombre);
  }
  return fp;
}

void escribir_archivo_y_cerrar(FILE* fp, struct complex vector[], int n){
  int i;
  for (i=0;i<n;i++){
    fprintf(fp, FORMATO_EN_ARCHIVO,vector[i].re,vector[i].im);
  }
  fclose (fp);
}


void escribir_modfft_archivo_y_cerrar(FILE* fp, struct complex vector[], int n, float fs){
  int i;
  printf("Escribir mod(FFT).\n");
  printf("Recibido fs=%f.\n",fs);
  for (i=n/2;i<n;i++){
    fprintf(fp, FORMATO_EN_ARCHIVO_MODFFT, ((float)(i-n/2)/n - 0.5)*fs, sqrt(pow(vector[i].re, 2.0) + pow(vector[i].im, 2.0)));
  }
  for (i=0;i<(n/2);i++){
    fprintf(fp, FORMATO_EN_ARCHIVO_MODFFT, ((float)i/n)*fs, sqrt(pow(vector[i].re, 2.0) + pow(vector[i].im, 2.0)));
  }
  
  fclose (fp);
  printf("Fin escribir mod(FFT).\n");
}




void escribir_fft_archivo_y_cerrar(FILE* fp, struct complex vector[], int n, float fs){
  int i;
  printf("Escribir FFT.\n");
  printf("Recibido fs=%f.\n",fs);
  for (i=n/2;i<n;i++){
    fprintf(fp, FORMATO_EN_ARCHIVO_FFT, ((float)(i-n/2)/n - 0.5)*fs, vector[i].re,vector[i].im);
  }
  for (i=0;i<(n/2);i++){
    fprintf(fp, FORMATO_EN_ARCHIVO_FFT,  ((float)i/n)*fs, vector[i].re,vector[i].im);
  }
  
  fclose (fp);
  printf("Fin escribir FFT.\n");
}




void cerrar_archivo(FILE* fp){
  fclose (fp);
}



    //FILE *fp3, *fp2, *fp;
    //fp  = (FILE*) abrir_archivo_out("senal_usada.txt");
    //fp2 = (FILE*) abrir_archivo_out("salida.txt");
    //fp3 = (FILE*) abrir_archivo_out("salidamod.txt");
    
    //escribir_archivo_y_cerrar(fp,vector, n);
    //printf("* Iniciado el procesamiento de FFT (%d muestras).\n", n);
    //tiempo = clock();
    //ffttras(senal, fft_res, n,np);            /* Procesar FFT. El vector X sólo se usa una vez.*/
    //tiempo = clock() - tiempo;
    //free(senal);
    //printf("* Procesamiento de FFT terminado.\n");
    //imprimir_maximo_modulo(fft_res, n, fs);
    //escribir_modfft_archivo_y_cerrar(fp3,vector2, n,fs);
    //printf("Listo! FFT tardo %d ciclos.\n",tiempo);


