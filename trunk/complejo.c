#include <stdlib.h>
#include <stdio.h>
#include "complejo.h"

void generar_coseno(struct complex vector[], int n, float fs, float f){
  int i=0;
  for(i=0;i<n;i++){
    vector[i].re = sin(PI2*(f/fs)*i);
    vector[i].im = 0;
  }
  printf("Coseno generado (f_sample=%fHz): f=%fHz\n",fs,f);
}

void generar_expcomp(struct complex vector[], int n, float fs, float f){
  int i=0;
  for(i=0;i<n;i++){
    vector[i].re = sin(PI2*(f/fs)*i);
    vector[i].im = cos(PI2*(f/fs)*i);
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



void imprimir_maximo_modulo(struct complex vector[], int n, float fs){
  int i;
  float maximo=0,candidato,fr_max;

  for (i=n/2;i<n;i++){
    candidato = sqrt(pow(vector[i].re, 2.0) + pow(vector[i].im, 2.0));
    if (candidato>=maximo){
      maximo = candidato;
      fr_max = (((float)(i-n/2)/n - 0.5)*fs);
    }
  }
  for (i=0;i<(n/2);i++){
    candidato = sqrt(pow(vector[i].re, 2.0) + pow(vector[i].im, 2.0));
    if (candidato>=maximo){
      maximo = candidato;
      fr_max = (((float)i/n)*fs);
    }
    
  }
  printf("El maximo se obtuvo en +/- %fHz.\n",fr_max);

}


