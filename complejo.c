#include <stdlib.h>
#include <stdio.h>
#include "complejo.h"
#define PI 3.14159265

float rnd(){
  return (((float)rand())/RAND_MAX);
}

void cargar_vector(struct complex vector[], int n, float fs, float f){
  int i=0;
  for(i=0;i<n;i++){
    vector[i].re = sin(2*PI*(f/fs)*i);
    vector[i].im = 0;
  }
  printf("Vector generado (f_sample=%fHz): f=%fHz\n",fs,f);
}

void imprimir_vector(struct complex vector[], int n){
  int i;
  printf("Vector: \n");
  for(i=0;i<n;i++){
    printf("    %d     %2.5f+j%2.5f\n",i,vector[i].re,vector[i].im);
  }  
  printf("Fin vector.\n");
}

