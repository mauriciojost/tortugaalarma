#ifndef COMPLEJO_H
#define COMPLEJO_H

#include <math.h>
struct complex
{
	double re;
	double im;
};

float rnd();
void cargar_vector(struct complex vector[], int n, float fs, float f);
void imprimir_vector(struct complex vector[], int n);

#endif

