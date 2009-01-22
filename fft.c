//#include <stdio.h>
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
  unsigned int len_bits;
  n_total=n;
  len_bits = (unsigned int)((double)log2((double)n_total));
  twiddle_factors = (struct complex*) malloc(n/2*sizeof(struct complex));
  struct complex w;
  float arg;

  for (i=0;i<n/2;i++){
    arg = PI2*i/n_total;
	  twiddle_factors[i].re = cos((double)arg);
	  twiddle_factors[i].im = sin((double)arg);

    //igcmplxp(w,  &);
  }

  for(i=0;i<semi_n;i++){ 
    mariposa     (&x[i], &x[i+semi_n], i);
  }
  
  fft(x,   semi_n,2);
  fft(x+semi_n, semi_n,2);
  
  for(i=0;i<n;i++){ 
    igcmplxp(&x[i],  &y[reversal_bit(i,len_bits)]);
  }
  
  free(twiddle_factors);
}


void fft(struct complex *x, unsigned int len, unsigned int profundidad)
{
  int i, N=len>>1;
  
  if (len>1){ 
    for(i=0;i<N;i++)
      mariposa(&x[i], &x[i+N], i*profundidad);
    
    fft(x,   N,profundidad*2);
    fft(x+N, N,profundidad*2);
  }
}


unsigned int reversal_bit(unsigned int a, unsigned int n){
  unsigned int i=0;
  unsigned int base=0;
  unsigned int mask = 1<<(n-1);
  unsigned int shift=1;

  //printf("base=%u, mask=%u, shift=%u, i=%u.\n",base,mask,shift,i);
  for (i=0;i<n;i++){
    base |= (mask & a)?shift:0;
    mask = mask >> 1;
    shift = shift << 1;
    //printf("base=%u, mask=%u, shift=%u, i=%u.\n",base,mask,shift,i);
  }
  return base;
}


