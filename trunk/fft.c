#include "fft.h"
#include "complejo.h"
#include "wcomplex512.h"
#include "revord512.h"
#include <stdio.h>

const int SIZE=512;


void mariposa(struct complex a, struct complex b, struct complex* A, struct complex* B, int wn)
{
  struct complex w;

  // A = (a+b)
  A->re = a.re+b.re;
  A->im = a.im+b.im;

  // B = (a-b)*W
  // B = (a-b)... 1° parte...
  B->re = a.re-b.re;
  B->im = a.im-b.im;
  
  // B = (a-b)*W  2° parte...
  float arg = 3.14159265*2*wn/512;
	w.re = cos((double)arg);
	w.im = sin((double)arg);
  multcmplxp(*B,w,B);

  // B = (a-b)*W  2° parte...
  //multcmplxp(*B,wcomplex[wn],B);

  
}


void ffttras(struct complex *x,struct complex *y)
{
  int i, N=SIZE>>1;
  printf("Antes de todo...\n");
  imprimir_vector_complejo(x,SIZE);
  for(i=0;i<N;i++)
  { 
    mariposa     (x[i],x[i+N] ,&x[i],&x[i+N],i);
  }
  printf("Antes de todo...\n");
  imprimir_vector_complejo(x,SIZE);
  
  fft(x,   N,2);
  fft(x+N, N,2);
  
  for(i=0;i<SIZE;i++)
  { 
    igcmplxp(x[i],  &y[revers(i)]);
  }
  
}


void fft(struct complex *x, int len, int profundidad)
{
  int i, N=len>>1;
  
  if (len>1)
  { 
    for(i=0;i<N;i++)
      mariposa(x[i],x[i+N],&x[i],&x[i+N],i*profundidad);
    
    fft(x,   N,profundidad*2);
    fft(x+N, N,profundidad*2);
  }
}

int revers(int pos)
{ 
  return vrev512[pos];
}


















/*
inline void mariposa_real(float a, float b, cmplx* A, cmplx* B, int wn)
{
  float aux;

  // A = (a+b)
  A->re = a+b;
  A->im = 0;

  // B = (a-b)*W
  // B = (a-b)... 1° parte...
  
  aux = a-b;
  
  // B = (a-b)*W  2° parte...
  
  B->im = aux * wcomplex[wn].im;
  B->re = aux * wcomplex[wn].re;
  
}
*/

