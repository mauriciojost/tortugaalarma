#include <stdio.h>
#include <stdlib.h>
#include "./../archivo.h"

#define MiB 1024*1024

typedef unsigned int uint;

int main(int argc, char *argv[]){

  //generar_archivo();

  char dominio='r';
  float fs=10000.0, f=333.0, dc=0, noise=0, fm=0;
  uint cant=1024;
  scomplex *senial;


  printf("Inicio del Generador de señales.\n");  
  printf("FORMATO:  %s  r|c  fs  f  n  [dc [ruido [fm]]].\n\n",argv[0]);

  if (argc>=5){
    //printf("argc=%u argv[0]='%s' argv[1]='%s'.\n",argc, argv[0], argv[1]);
    dominio = argv[1][0];
    //printf("Compleja o real (%c). (argv[1]='%s').\n", dominio, argv[1]);
    sscanf(argv[2],"%f",&fs);
    //printf("Frecuencias fs=%f (argv[2]='%s').\n",fs,argv[2]);
    sscanf(argv[3],"%f",&f);
    //printf("Frecuencias f=%f (argv[3]='%s').\n",f ,argv[3]);
    sscanf(argv[4],"%u",&cant);
    //cant = 1024 * cant;
    //printf("Cantidad=%u  (argv[4]='%s').\n",cant,argv[4]);
  }

  senial = (scomplex*) malloc(cant*sizeof(scomplex));          

  if(dominio == 'c'){
    generar_expcomp(senial,cant,fs,f);
  }else{
    generar_coseno(senial,cant,fs,f);    
  }
  

  if (argc>=6){
    sscanf(argv[5],"%f",&dc);
    //printf("dc=%f  (argv[5]='%s').\n",dc,argv[5]);    
  }
  montardc(senial,cant,dc);
  //printf("dc=%f.\n", dc);

  if (argc>=6){
    sscanf(argv[6],"%f",&noise);
    //printf("noise=%f  (argv[6]='%s').\n",noise,argv[6]);
  }
  montar_ruido(senial, cant,noise);
  //printf("ruido=%f.\n", noise);

  if (argc>=6){
    sscanf(argv[7],"%f",&fm);
    //printf("fm=%f  (argv[7]='%s').\n",fm,argv[7]);
  }
  modular(senial, cant, fs, fm);
  //printf("fm=%f.\n", fm);

  printf("Dm=%c fs=%1.1f f=%1.1f cnt=%u(%uK) dc=%1.1f ns=%1.1f mod=%1.1f.\n", dominio,fs,f,cant,cant/1024,dc,noise,fm);

  printf("*** Guardando archivo...\n");
  escribir_archivo_t("entrada.txt", senial, cant, fs);
  

  free(senial);
  printf("Fin!!!\n");
  return 0;
}

