
#ifndef	_FFTM_H
#define _FFTM_H

#include "complejo.h"


void mariposa(struct complex *a, struct complex *b, unsigned int wn);
void fft(struct complex x[], unsigned int len, unsigned int profundidad, unsigned int myrank, unsigned int posicion);
void ffttras(struct complex *x,struct complex *y, unsigned int n, unsigned int np);

unsigned int reversal_bit(unsigned int a, unsigned int n_bits);
#endif /* _FFTM_H */
