#include <stdlib.h>
#include <stdio.h>
#include "complejo.h"
#define PI 3.14159265

float rnd(){
  return (((float)rand())/RAND_MAX);
}

void cargar_vector_complejo(struct complex vector[], int n, float fs, float f){
  int i=0;
  for(i=0;i<n;i++){
    vector[i].re = sin(2*PI*(f/fs)*i);
    vector[i].im = 0;
  }
  printf("Vector generado (f_sample=%fHz): f=%fHz\n",fs,f);
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
  printf("Maximo modulo.\n");
  printf("Recibido fs=%f.\n",fs);
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
  printf("Fin de maximo modulo.\n");
}




void igcmplxp(struct complex a, struct complex *b)
{
  b->re = a.re;
  b->im = a.im;
}

void multcmplxp(struct complex a, struct complex b, struct complex *c)
{
  struct complex d;
  /*             A . B               */
  /*     (ar+ai*j) . (br+bi*j)       */
  /*  ar.br-ai.bi  + (ar.bi+ai.br).j */
  /*       cr      +      ci.j       */
  /*               C                 */
  
  d.re = (a.re*b.re) - (a.im*b.im);
  d.im = (a.re*b.im) ;
  d.im = d.im + (a.im*b.re);
  igcmplxp(d,c);
  
}


void sumcmplxp(struct complex a, struct complex b, struct complex *c)
{
  c->re = a.re+b.re;
  c->im = a.im+b.im;
}

void rescmplxp(struct complex a, struct complex b, struct complex *c)
{
  c->re = a.re-b.re;
  c->im = a.im-b.im;
}



