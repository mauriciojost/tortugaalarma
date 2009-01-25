
#ifndef	_FFTM_H
#define _FFTM_H

#include "complejo.h"

typedef unsigned int uint;
typedef struct complex scomplex;
inline void mariposa(struct complex *a, struct complex *b, uint wn);
void fft(struct complex x[], uint len, uint profundidad, uint myrank, uint posicion);
void ffttras(struct complex *x,struct complex *y, uint n, uint np);
uint reversal_bit(uint a, uint n_bits);
uint logbase2(uint arg);
uint responsab(uint posic);


#endif /* _FFTM_H */
