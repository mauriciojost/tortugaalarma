#ifndef COMPLEJO_H
#define COMPLEJO_H

#include <math.h>
struct complex
{
	double re;
	double im;
};

float rnd();
void cargar_vector_complejo(struct complex vector[], int n, float fs, float f);
void imprimir_vector_complejo(struct complex vector[], int n);
void imprimir_maximo_modulo(struct complex vector[], int n, float fs);

#endif

