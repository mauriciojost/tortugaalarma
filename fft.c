#include <stdio.h>
#include <stdlib.h>
#include "complejo.h"
#include "fft.h"#include <math.h>

unsigned int n_total;
struct complex *vector2;
struct complex *twiddle_factors;

void mariposa(struct complex *a, struct complex *b, unsigned int wn){
  // A = (a+b)
  a->re = a->re + b->re;
  a->im = a->im + b->im;

  // B = (a-b)*W
  // B = (a-b)... 1° parte...
  b->re = a->re - b->re;
  b->im = a->im - b->im;
  
  // B = (a-b)*W  2° parte...
  multcmplxp(b,&twiddle_factors[wn],b);
}

void ffttras(struct complex *x,struct complex *y, unsigned int n){
  unsigned  i, semi_n=n>>1;
  unsigned int len_bits, aux;
  float arg;

  n_total=n;
  len_bits = (unsigned int)((double)log2((double)n_total));
  twiddle_factors = (struct complex*) malloc(n/2*sizeof(struct complex));


  for (i=0;i<n/2;i++){
    arg = PI2*i/n_total;
	  twiddle_factors[i].re = cos((double)arg);
	  twiddle_factors[i].im = sin((double)arg);
  }

  for(i=0;i<semi_n;i++){ 
    mariposa     (&x[i], &x[i+semi_n], i);
  }
  
  fft(x,   semi_n,2);
  fft(x+semi_n, semi_n,2);
  
  struct complex interm;
  for(i=0;i<n;i++){ 
    aux = reversal_bit(i,len_bits);
/*
    interm.re = x[i].re;
    interm.im = x[i].im;

    x[i].re = x[aux].re;
    x[i].im = x[aux].im;    
    
    x[aux].re = interm.re;
    x[aux].im = interm.im;
*/
    y[aux].re = x[i].re;
    y[aux].im = x[i].im;  
  }
  
  free(twiddle_factors);
}


void fft(struct complex *x, unsigned int len, unsigned int profundidad){
  unsigned int i, N=len>>1;
  
  if (len>1){ 
    for(i=0;i<N;i++)
      mariposa(&x[i], &x[i+N], i*profundidad);
    
    fft(x,   N,profundidad*2);
    fft(x+N, N,profundidad*2);
  }
}


/* Algoritmo de bit-reverso. 
 * Es utilizado en el ordenamiento de los datos obtenidos en el proceeso.
 */
unsigned int reversal_bit(unsigned int a, unsigned int n){
  unsigned int i=0;               /* Índice de bucle.                                                     */
  unsigned int base=0;            /* Base sobre la que se escriben los bits en orden inverso.             */
  unsigned int mask = 1<<(n-1);   /* Máscara utilizada para analizar a la palabra original.               */
  unsigned int shift=1;           /* Registro con bit en desplazamiento inverso a mask, para setear base. */

  for (i=0;i<n;i++){              /* n es la cantidad de bits necesarios para direccionar las muestras.   */
    base |= (mask & a)?shift:0;   /* La base es seteada según shift (<-), sólo si hay un 1 en la posición */
    mask = mask >> 1;             /* dada  por mask (->). */
    shift = shift << 1;
  }
  return base;
}


