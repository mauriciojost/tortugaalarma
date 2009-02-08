#include <stdlib.h>
#include <stdio.h>
#include "complejo.h"

void generar_coseno(struct complex vector[], int n, float fs, float f){
  int i=0;
  for(i=0;i<n;i++){
    vector[i].re = cos(PI2*(f/fs)*i);
    vector[i].im = 0;
  }
  printf("Coseno generado (f_sample=%fHz): f=%fHz\n",fs,f);
}

void generar_expcomp(struct complex vector[], int n, float fs, float f){
  int i=0;
  for(i=0;i<n;i++){
    //(PI2*(f/fs)*i);

    vector[i].re = cos(PI2*(f/fs)*i);
    vector[i].im = sin(PI2*(f/fs)*i);
  }
  printf("Exp. compleja generada (f_sample=%fHz): f=%fHz\n",fs,f);
}


void imprimir_vector_complejo(struct complex vector[], int n){
  int i;
  printf("Vector: \n");
  for(i=0;i<n;i++){
    printf("    %d     %2.5f+j%2.5f\n",i,vector[i].re,vector[i].im);
  }  
  printf("Fin vector.\n");
}


uint valid(uint i, uint n){
  return i%n;
}

float getfrec(uint i, uint n, float fs){
  if (i>(n/2)){
    return (((float)(i-n/2)/n - 0.5)*fs);
  }else{
    return (((float)i/n)*fs);
  }
}

void imprimir_maximo_modulo(struct complex vector[], int n, float fs){
  int i;
  float maximo=0,candidato, anterior, siguiente,fr_max;
  printf("\nResumen de máximos:\n");
  for (i=0;i<n;i++){
    candidato = sqrt(pow(vector[valid(i,n)].re, 2.0) + pow(vector[valid(i,n)].im, 2.0));
    anterior = sqrt(pow(vector[valid(i-1,n)].re, 2.0) + pow(vector[valid(i-1,n)].im, 2.0));
    siguiente = sqrt(pow(vector[valid(i+1,n)].re, 2.0) + pow(vector[valid(i+1,n)].im, 2.0));
    if ((candidato>=(siguiente+3)) && (candidato>=(anterior+3))){
      printf(" .Maximo en %fHz.\n",getfrec(i, n, fs));
    }
  }
  printf("Fin de resumen.\n");  

}

/*
void imprimir_maximo_modulo(struct complex vector[], int n, float fs){
  int i;
  float maximo=0,candidato,fr_max;

  for (i=0;i<n;i++){
    candidato = sqrt(pow(vector[i].re, 2.0) + pow(vector[i].im, 2.0));
    if (candidato>=maximo){
      maximo = candidato;
      fr_max = getfrec(i, n, fs);
    }
  }
  printf("El maximo se obtuvo en +/- %fHz.\n",fr_max);

}

*/
