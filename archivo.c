#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "archivo.h"
#include "complejo.h"


//#define FORMATO_EN_ARCHIVO     "%2.2f+j*%2.2f\n"
//#define FORMATO_EN_ARCHIVO_FFT "%1.1fHz-> %2.2f+j*%2.2f\n"
//#define FORMATO_EN_ARCHIVO_MODFFT "%1.1fHz-> %2.2f\n"

#define FORMATO_EN_ARCHIVO     "%f+%fj\n"
#define FORMATO_EN_ARCHIVO_SN  "%f+%fj"

#define FORMATO_EN_ARCHIVOT     "%f+%fj\n"
#define FORMATO_EN_ARCHIVOT_SN  "%f+%fj"

#define FORMATO_EN_ARCHIVO_FFT "%f -> %f + %fj \n"
#define FORMATO_EN_ARCHIVO_FFT_SN "%f -> %f + %fj"

#define FORMATO_EN_ARCHIVO_MODFFT    "%1.1fHz-> %2.2f\n"
#define FORMATO_EN_ARCHIVO_MODFFT_SN "%1.1fHz-> %2.2f"


FILE* abrir_archivo_out(char *nombre){
  FILE* fp = fopen (nombre, "w");
  if (fp==NULL){
    printf("Error al abrir el archivo '%s'.\n",nombre); exit(-1);
  }else{
    printf("Archivo '%s' abierto correctamente. Listo para su escritura.\n",nombre);
  }
  return fp;
}

FILE* abrir_archivo_in(char *nombre){
  FILE* fp = fopen (nombre, "r");
  if (fp==NULL){
    printf("Error al abrir el archivo '%s'.\n",nombre); exit(-1);
  }else{
    printf("Archivo '%s' abierto correctamente. Listo para su lectura.\n",nombre);
  }
  return fp;
}

void escribir_archivo_t(char* nombre, struct complex vector[], unsigned int n, float fs){
  int i;
  FILE* fp = abrir_archivo_out(nombre);

  fprintf(fp, "%f\n",fs); fprintf(fp, "%u\n",n); /* Escribe fs y luego len. */
  for (i=0;i<n;i++){
    fprintf(fp, FORMATO_EN_ARCHIVOT,vector[i].re,vector[i].im);
  }
  fclose (fp);
}

void abrir_archivo_t(char *nombre, scomplex* vector[], unsigned int* n, float* fs){
  char buffer[101];
  unsigned int i;
  float f,rea,ima;
  FILE *fp = abrir_archivo_in(nombre);

  
  fgets(buffer, 100,fp); sscanf(buffer,"%f",fs); /* Lee fs.  */
  fgets(buffer, 100,fp); sscanf(buffer,"%u",n);  /* Lee len. */

  (*vector) = (scomplex*) malloc((*n)*sizeof(scomplex));          
  printf("Vector de %u elementos creado. Leyendo...\n",*n);

  i=0;
  while(!feof(fp)){
    fgets(buffer, 100,fp);  
    sscanf(buffer,FORMATO_EN_ARCHIVO_SN, &rea, &ima);
    (*vector)[i].re = rea; (*vector)[i++].im = ima;
    if (i==*n){break;}
  }
  printf("Lectura lista. Obtenidos %u elementos.\n",i);
  fclose(fp);  
}

void escribir_archivo_mf(char* nombre, scomplex vector[], uint n, float fs){
  uint i;
  FILE *fp = abrir_archivo_out(nombre);
  printf("Escribiendo mod(FFT).\n");

  for (i=n/2;i<n;i++)
    fprintf(fp, FORMATO_EN_ARCHIVO_MODFFT, ((float)(i-n/2)/n - 0.5)*fs, sqrt(pow(vector[i].re, 2.0) + pow(vector[i].im, 2.0)));
  for (i=0;i<(n/2);i++)
    fprintf(fp, FORMATO_EN_ARCHIVO_MODFFT, ((float)i/n)*fs, sqrt(pow(vector[i].re, 2.0) + pow(vector[i].im, 2.0)));
  
  fclose (fp);
}

void escribir_archivo_f(char *nombre, scomplex vector[], uint n, float fs){
  uint i;
  FILE *fp = abrir_archivo_out(nombre);
  printf("Escribiendo FFT.\n");

  for (i=n/2;i<n;i++)
    fprintf(fp, FORMATO_EN_ARCHIVO_FFT, ((float)(i-n/2)/n - 0.5)*fs, vector[i].re,vector[i].im);

  for (i=0;i<(n/2);i++)
    fprintf(fp, FORMATO_EN_ARCHIVO_FFT,  ((float)i/n)*fs, vector[i].re,vector[i].im);
  
  fclose (fp);
}

