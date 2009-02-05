#ifndef ARCHIVO_H
#define ARCHIVO_H
#include <stdio.h>
#include <stdlib.h>
#include "complejo.h"

FILE* abrir_archivo_in(char *nombre);
FILE* abrir_archivo_out(char *nombre);
void escribir_archivo_mf(char* nombre, scomplex vector[], uint n, float fs);
void escribir_archivo_f(char *nombre, scomplex vector[], uint n, float fs);
void escribir_archivo_t(char* nombre, scomplex vector[], uint n, float fs);
void abrir_archivo_t(char *nombre, scomplex* vector[], uint* n, float* fs);

void generar_archivo();

void modular_complejo(scomplex* senal, uint n, float fs, float fm);
void generar_base_unitaria(scomplex* senal, uint n);
void montar_ruido(scomplex* senal, uint n, float factor);
void montardc(scomplex* senal, uint n, float dc);
void modular(scomplex* senal, uint n, float fs, float fm);


#endif
