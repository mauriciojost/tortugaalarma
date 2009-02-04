#ifndef ACC_FFT_H
#define ACC_FFT_H

#include <stdio.h>
#include "fft.h"

extern uint myrank, np_total, nro_muestras_total;
extern scomplex *twiddle_factors;

inline void mariposa(scomplex *a, scomplex *b, uint wn);
uint corregir_np(uint np);
uint responsab(uint posic);
uint responsab_ord(uint uP);
uint reversal_bit(uint a, uint n_bits);
uint logbase2(uint arg);
double calcular_suma_verif(uint semi_len, scomplex* s);

#endif //ACC_FFT_H
