
#ifndef	_FFTM_H
#define _FFTM_H

#include "complejo.h"

int revers(int pos);
int reverslento(int pos, int bits);
void mariposa(struct complex *a, struct complex *b, struct complex* A, struct complex* B, unsigned int wn);
void fft(struct complex x[], unsigned int len, unsigned int profundidad);
void ffttras(struct complex *x,struct complex *y, unsigned int n);

unsigned int reversal_bit(unsigned int a, unsigned int n_bits);
#endif /* _FFTM_H */
