
#ifndef	_FFTM_H
#define _FFTM_H

#include "complejo.h"

void fft(scomplex *x, scomplex *y, uint n, uint np);
void dft(scomplex x[], uint len, uint profundidad, uint myrank, uint posicion);
void ordenar_bit_reversal_parcial(scomplex* senal, scomplex* fft_res, uint n, uint np, uint myrank);
void derivar_trabajo(scomplex* senal, uint semi_len, uint profundidad, uint respons, uint pos, uint myrank);
void fft_recibir_tarea();

#endif /* _FFTM_H */
