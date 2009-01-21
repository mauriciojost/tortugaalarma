#ifndef COMPLEJO_H
#define COMPLEJO_H

#include <math.h>

struct complex
{
	double re;
	double im;
};

/*
typedef struct
{
	double re;
	double im;
}complex;
*/


float rnd();
void cargar_vector_complejo(struct complex vector[], int n, float fs, float f);
void imprimir_vector_complejo(struct complex vector[], int n);
void imprimir_maximo_modulo(struct complex vector[], int n, float fs);

#endif



#ifndef	_CMPLX_H
#define _CMPLX_H

void multcmplxp(struct complex a, struct complex b, struct complex *c);
void sumcmplxp(struct complex a, struct complex b, struct complex *c);
void rescmplxp(struct complex a, struct complex b, struct complex *c);
void igcmplxp(struct complex a, struct complex *b);


#endif /* _CMPLX_H */

