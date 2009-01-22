
#ifndef	_FFTM_H
#define _FFTM_H

#include "complejo.h"

int revers(int pos);
int reverslento(int pos, int bits);
void mariposa(struct complex a, struct complex b, struct complex* A, struct complex* B, int wn);
void fft(struct complex x[], int len, int profundidad);
void ffttras(struct complex *x,struct complex *y, int n);
void mariposa_real(float a, float b, struct complex* A, struct complex* B, int wn);
int reversal_bit(int a, int n);
#endif /* _FFTM_H */
