#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#define N 512*8*8*8*2

struct complex
{
	double re;
	double im;
};


float rnd(){
  return (((float)rand())/RAND_MAX);
}

void cargar_vector(struct complex vector[]){
  int i;
  for(i=0;i<N;i++){
    vector[i].re = rnd();
    vector[i].im = rnd();
  }
}

void imprimir_vector(struct complex vector[]){
  int i;
  printf("Vector: \n");
  for(i=0;i<N;i++){
    printf("    %d     %2.5f+j%2.5f\n",i,vector[i].re,vector[i].im);
  }  
  printf("Fin vector.\n");
}

int main(){

  struct complex cx[N];
  int lx=N;
	float signi=1, scale=1;

  cargar_vector(cx);
  //imprimir_vector(cx);
	fft(lx, cx, signi, scale);
  //imprimir_vector(cx);
  printf("Listo!\n");
  return 0;

}
