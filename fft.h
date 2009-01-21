
#ifndef	_FFTM_H
#define _FFTM_H

#include "complejo.h"

int revers(int pos);
int reverslento(int pos, int bits);
void mariposa(struct complex a, struct complex b, struct complex* A, struct complex* B, int wn);
void setear();
int logbase2(int a);
void fft(struct complex x[], int len, int profundidad);
void leer();
void ffttras();
void mariposa_real(float a, float b, struct complex* A, struct complex* B, int wn);

#endif /* _FFTM_H */
