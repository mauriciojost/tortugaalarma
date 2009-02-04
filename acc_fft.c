#include "acc_fft.h"

unsigned int corregir_np(unsigned int np){
	int i;
	for (i=1;i<=np;i*=2){}
	return i/2;
}


/* Establece la responsabilidad de cada etapa de la FFT.
 * Retorna el uP asociado a la posición del vector de muestras.
 */
uint responsab(uint posic){  
  uint myrank;
  uint bloque_por_uP = (uint)(nro_muestras_total/np_total);
  
  myrank = posic / bloque_por_uP;
  printf("   Solicitada resp. (posic=%u -> resp=%u).\n",posic,myrank);
  return myrank;
}


/* Establece la responsabilidad para un uP en el orden reverso.
 * Retorna la posición del vector de muestras asociado al uP dado (myrank).
 */
uint responsab_ord(uint uP){  
  uint posic;
  uint bloque_por_uP = (uint)(nro_muestras_total/np_total);
  posic = myrank* bloque_por_uP;
  return posic;
}



/* Algoritmo de bit-reverso. 
 * Es utilizado en el ordenamiento de los datos obtenidos en el proceeso.
 */
uint reversal_bit(uint a, uint n){
  uint i=0;               /* Índice de bucle.                                                     */
  uint base=0;            /* Base sobre la que se escriben los bits en orden inverso.             */
  uint mask = 1<<(n-1);   /* Máscara utilizada para analizar a la palabra original.               */
  uint shift=1;           /* Registro con bit en desplazamiento inverso a mask, para setear base. */

  for (i=0;i<n;i++){              /* n es la cantidad de bits necesarios para direccionar las muestras.   */
    base |= (mask & a)?shift:0;   /* La base es seteada según shift (<-), sólo si hay un 1 en la posición */
    mask = mask >> 1;             /* dada  por mask (->). */
    shift = shift << 1;
  }
  return base;
}


uint logbase2(uint arg){
  uint i=0, multip=1;
  while(multip<arg){
    multip<<=1;
    i++;
  }
  return i;
}

inline void mariposa(scomplex *a, scomplex *b, uint wn){  scomplex d,aux,twf;

  //printf("                        marip_e: %1.2f+j%1.2f %1.2f+%1.2f tw=%u.\n",a->re,a->im,b->re,b->im,wn);

  twf.re = twiddle_factors[wn].re;
  twf.im = twiddle_factors[wn].im;
  //printf("                              twf_e: %1.2f+j%1.2f.\n",twf.re,twf.im);
  // A = (a+b)
  aux.re = a->re + b->re; aux.im = a->im + b->im;

  // B = (a-b)*W
  b->re = a->re - b->re; b->im = a->im - b->im;
  d.re = (b->re*twf.re) - (b->im*twf.im);
  b->im = (b->re * twf.im) + (b->im*twf.re);
  b->re = d.re;
  a->re = aux.re; a->im = aux.im;
  //printf("                        marip_s: %1.2f+j%1.2f %1.2f+%1.2f tw=%u.\n",a->re,a->im,b->re,b->im,wn);
}


/* Calcula la suma de verificación de un vector complejo. */
float calcular_suma_verif(uint semi_len, scomplex* senal){
  uint aux;
  float verif=0;
  for(aux=0;aux<semi_len;aux++){
    verif+= (senal[aux].re + senal[aux].im);
  }
  return verif;
}



