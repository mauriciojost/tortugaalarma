#include <stdio.h>
#include <stdlib.h>
#include "complejo.h"
#include "fft.h"#include <math.h>

unsigned int logbase2(unsigned int arg);
unsigned int responsab(unsigned int posic);

unsigned int nro_muestras_total;
unsigned int np_total;

struct complex *vector2;
struct complex *twiddle_factors;

void mariposa(struct complex *a, struct complex *b, unsigned int wn){  struct complex d,twf;
  twf.re = twiddle_factors[wn].re;
  twf.im = twiddle_factors[wn].im;

  // A = (a+b)
  a->re = a->re + b->re; a->im = a->im + b->im;

  // B = (a-b)*W
  b->re = a->re - b->re; b->im = a->im - b->im;
  d.re = (b->re*twf.re) - (b->im*twf.im);
  b->im = (b->re * twf.im) + (b->im*twf.re);
  b->re = d.re;
}

void ffttras(struct complex *senal,struct complex *fft_res, unsigned int n,unsigned int nproc){
  unsigned  i, semi_n=n>>1;
  unsigned int len_bits, aux;
  float arg;

  nro_muestras_total=n;
  np_total = nproc;
  //len_bits = (unsigned int)((double)log2((double)nro_muestras_total));
  len_bits = logbase2(nro_muestras_total);
  twiddle_factors = (struct complex*) malloc((n>>1)*sizeof(struct complex));

  for (i=0;i<(n>>1);i++){
    arg = PI2*i/nro_muestras_total;
	  twiddle_factors[i].re = cos((double)arg);
	  twiddle_factors[i].im = sin((double)arg);
  }

  for(i=0;i<semi_n;i++){ 
    mariposa     (&senal[i], &senal[i+semi_n], i);
  }
  
  fft(senal,   semi_n,2,0, 0);
  fft(senal+semi_n, semi_n,2,responsab(semi_n), semi_n);
  
  struct complex interm;
  for(i=0;i<n;i++){ 
    aux = reversal_bit(i,len_bits);
    fft_res[aux].re = senal[i].re;
    fft_res[aux].im = senal[i].im;  
  }
  
  free(twiddle_factors);
}


void fft(struct complex *senal, unsigned int mylen, unsigned int profundidad, unsigned int myrank, unsigned int posicion){
  unsigned int i, semi_len=mylen>>1;
  unsigned int posicion_segundo_hijo, respons;
  
  //printf("fft: uP=%u/%d. Pos=%u. ",myrank,np_total, posicion);
  //printf("len=%u limite=%u ", mylen, nro_muestras_total/np_total);
  if (mylen>1){ 
    for(i=0;i<semi_len;i++){
      mariposa(&senal[i], &senal[i+semi_len], i*profundidad);
    }
    
    if (mylen <= (nro_muestras_total/np_total)){
      posicion_segundo_hijo = posicion;  
      respons = myrank;
      //printf("Sigo yo!!!!!!!!.\n");  
    }else{
      posicion_segundo_hijo = posicion+(mylen>>1);  
      respons = responsab(posicion_segundo_hijo);
      //printf("Delego a %u (yo),y %u...\n", myrank, respons);

    }

    fft(senal,   semi_len,profundidad*2,myrank,posicion);
    fft(senal+semi_len, semi_len,profundidad*2,myrank,posicion_segundo_hijo);    
  }else{
    //printf("Corto yo!!!!!!!!.\n");
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




unsigned int calcular_resp(unsigned int np, unsigned int myrank, unsigned int etapa){
  printf("np=%u, myrank=%u, etapa=%u\n",np,myrank,etapa);
}


unsigned int logbase2(unsigned int arg){
  unsigned int i=0, multip=1;
  while(multip<arg){
    multip*=2;
    i++;
  }
  return i;
}


unsigned int responsab(unsigned int posic){  
  unsigned int myrank;
  unsigned int bloque = (unsigned int)(nro_muestras_total/np_total);
  
  myrank = posic / bloque;
  printf("   Solicitada resp. (posic=%u -> resp=%u).\n",posic,myrank);
  return myrank;
}
