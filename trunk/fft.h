
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
uint reversal_bit(uint a, uint n_bits);
uint logbase2(uint arg);
uint responsab(uint posic);

void fft_recibir_tarea();
uint responsab_ord(uint uP);
void ordenar_bit_reversal_parcial(scomplex* senal, scomplex* fft_res, uint n, uint np, uint myrank);
float calcular_suma_verif(uint semi_len, scomplex* senal);
void derivar_trabajo(scomplex* senal, uint semi_len, uint profundidad, uint respons, uint pos, uint myrank);



#endif /* _FFTM_H */
