
#ifndef	_FFTM_H
#define _FFTM_H

#include "complejo.h"

typedef unsigned int uint;


inline void mariposa(scomplex *a, scomplex *b, uint wn);
void dft(scomplex x[], uint len, uint profundidad, uint myrank, uint posicion);
void fft(scomplex *x, scomplex *y, uint n, uint np);
uint reversal_bit(uint a, uint n_bits);
uint logbase2(uint arg);
uint responsab(uint posic);


#endif /* _FFTM_H */
