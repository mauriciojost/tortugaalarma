#ifndef ARCHIVO_H
#define ARCHIVO_H
#include <stdio.h>
#include <stdlib.h>
#include "complejo.h"
#define FORMATO_EN_ARCHIVO     "%2.5f + j%2.5f\n"
#define FORMATO_EN_ARCHIVO_FFT "%f1.1Hz -> %2.5f + j%2.5f\n"
#define FORMATO_EN_ARCHIVO_MODFFT "%f1.1Hz -> %2.5f\n"

FILE* abrir_archivo_in(char *nombre);
FILE* abrir_archivo_out(char *nombre);
void escribir_archivo_y_cerrar(FILE* fp, struct complex vector[], int n);
void escribir_fft_archivo_y_cerrar(FILE* fp, struct complex vector[], int n, float fs);
void cerrar_archivo(FILE* fp);
void escribir_modfft_archivo_y_cerrar(FILE* fp, struct complex vector[], int n, float fs);

#endif
