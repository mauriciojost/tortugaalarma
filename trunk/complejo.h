#ifndef COMPLEJO_H
#define COMPLEJO_H

#define PI2 3.14159265*2
#include <math.h>

struct complex
{
	double re;
	double im;
};

typedef struct complex scomplex;


void cargar_vector_complejo(struct complex vector[], int n, float fs, float f);
void imprimir_vector_complejo(struct complex vector[], int n);
void imprimir_maximo_modulo(struct complex vector[], int n, float fs);

#endif



