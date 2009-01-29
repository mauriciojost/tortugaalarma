#include <stdio.h>
#include <stdlib.h>
#include "complejo.h"
#include "fft.h"
#include <mpi.h>
#define ROOT 0

//#define MPI_TAG_MUESTRAS 55

/* Variables globales. No son modificadas luego de su asinación inicial. 
 * No representan problemas de concurrencia. 
 */
uint nro_muestras_total;  /* Cantidad de muestras a ser procesadas. Cumple nro=2^k (k entero).  */
uint np_total;            /* Cantidad de procesadores a ser utilizados.                         */

unsigned int nprocs, myrank;
MPI_Status status;
MPI_Datatype complex_MPI;

scomplex *vector2;
scomplex *twiddle_factors;

void fft_recibir_tarea();
float calcular_suma_verif(uint semi_len, scomplex* senal);
void derivar_trabajo(scomplex* senal, uint semi_len, uint profundidad, uint respons, uint pos, uint myrank);

inline void mariposa(scomplex *a, scomplex *b, uint wn){  scomplex d,aux,twf;

  //printf("                        marip_e: %1.2f+j%1.2f %1.2f+%1.2f tw=%u.\n",a->re,a->im,b->re,b->im,wn);

  twf.re = twiddle_factors[wn].re;
  twf.im = twiddle_factors[wn].im;
  //printf("                              twf_e: %1.2f+j%1.2f.\n",twf.re,twf.im);
  // A = (a+b)
  aux.re = a->re + b->re; aux.im = a->im + b->im;

  // B = (a-b)*W
  b->re = a->re - b->re; b->im = a->im - b->im;
  d.re = (b->re*twf.re) - (b->im*twf.im);
  b->im = (b->re * twf.im) + (b->im*twf.re);
  b->re = d.re;
  a->re = aux.re; a->im = aux.im;
  //printf("                        marip_s: %1.2f+j%1.2f %1.2f+%1.2f tw=%u.\n",a->re,a->im,b->re,b->im,wn);
}

void fft(scomplex *senal,scomplex *fft_res, uint n,uint nproc){
  uint  i, semi_n=n>>1;
  uint len_bits, aux;
  float arg;

  nro_muestras_total=n;
  np_total = nproc;
  len_bits = logbase2(nro_muestras_total);
  twiddle_factors = (scomplex*) malloc((n>>1)*sizeof(scomplex));

  for (i=0;i<(n>>1);i++){
    arg = PI2*i/n;
	  twiddle_factors[i].re = cos((double)arg);
	  twiddle_factors[i].im = sin((double)arg);
  }


  MPI_Type_contiguous(2, MPI_DOUBLE, &complex_MPI);
  MPI_Type_commit(&complex_MPI);
  MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
  MPI_Comm_size(MPI_COMM_WORLD, &nprocs);  


  if (myrank == ROOT){

    printf("ROOT.- Calculando primera etapa...\n");
    for(i=0;i<semi_n;i++){ 
      mariposa(&senal[i], &senal[i+semi_n], i);
    }  
    printf("ROOT.- Primera mitad procesada a %u (%u elementos)...\n",responsab(semi_n), semi_n);
    derivar_trabajo(senal+semi_n, semi_n, 2, responsab(semi_n), semi_n, 0);

    printf("ROOT.- Primera llamada a FFT...\n");
    dft(senal, semi_n, 2, 0, 0);  

    uint bloque = nro_muestras_total/np_total;

    printf("ROOT.- Esperando vectores (%u elementos c/u)...\n", bloque);

    MPI_Gather(senal, bloque, complex_MPI, senal, bloque, complex_MPI, ROOT, MPI_COMM_WORLD);

    printf("ROOT.- Ordenando...\n");
    scomplex interm;
    for(i=0;i<n;i++){ 
      aux = reversal_bit(i,len_bits);
      fft_res[aux].re = senal[i].re;
      fft_res[aux].im = senal[i].im;  
    }

    free(twiddle_factors);
    printf("ROOT.- Listo.\n");
  }else{
    fft_recibir_tarea();
    //MPI_Recv(senal+semi_n, semi_n, complex_MPI, ROOT, 123, MPI_COMM_WORLD, &status);
    // Recibir trama especial de tarea.
    // Hacer FFT.
    // Responder al ROOT.
  }
}


void fft_recibir_tarea(){
  uint aux, len, prof, dest, pos, orig;
  
  scomplex *vector_aux;



  printf("* %u.- Espero a recibir tarea...\n", myrank);
  MPI_Recv(&len, 1, MPI_UNSIGNED, MPI_ANY_SOURCE, 123, MPI_COMM_WORLD, &status);  /* Longitud.     */
  MPI_Recv(&prof, 1, MPI_UNSIGNED, MPI_ANY_SOURCE, 123, MPI_COMM_WORLD, &status); /* Profundidad.  */
  MPI_Recv(&dest, 1, MPI_UNSIGNED, MPI_ANY_SOURCE, 123, MPI_COMM_WORLD, &status); /* Destino.      */
  MPI_Recv(&pos, 1, MPI_UNSIGNED, MPI_ANY_SOURCE, 123, MPI_COMM_WORLD, &status);  /* Posicion.     */     
  MPI_Recv(&orig, 1, MPI_UNSIGNED, MPI_ANY_SOURCE, 123, MPI_COMM_WORLD, &status); /* Origen.       */

  printf("* %u.- Recibida tarea (orig=%u prof=%u dest=%u pos=%u orig=%u).\n", myrank,orig,prof,dest,pos,orig);

  vector_aux = (scomplex*) malloc(len*sizeof(scomplex));
  MPI_Recv(vector_aux, len, complex_MPI, MPI_ANY_SOURCE, 123, MPI_COMM_WORLD, &status);  /* Origen.       */


  //printf("* %u.- Recibi datos con exito (verif=%5.9f). Calculando FFT...\n", myrank,calcular_suma_verif(len, vector_aux));
  printf("* %u.- Recibi datos con exito. Calculando FFT...\n", myrank);
  dft(vector_aux, len,prof,myrank,pos);
  printf("* %u.- Calcule FFT con exito. Enviando a ROOT %u elementos...\n", myrank, nro_muestras_total/np_total);
  uint bloque = nro_muestras_total/np_total;
  MPI_Gather(vector_aux, bloque , complex_MPI, NULL, 0, complex_MPI, ROOT, MPI_COMM_WORLD);
  printf("* %u.- Envie resultado a ROOT con exito.\n", myrank);

}


/* Función de FFT con recursividad.
 * Establece las responsabilidades para cada uP.
 */
void dft(scomplex *senal, uint mylen, uint profundidad, uint myrank, uint posicion){
  uint i, semi_len=mylen>>1;
  uint posicion_segundo_hijo, respons;
  
  //printf("\nFFT entradas ->\n  * uP=%u/%d. Pos=%u ",myrank,np_total, posicion);
  //printf("len=%u prof=%u\n  * Vector: ", mylen, profundidad);

  //for(i=0;i<mylen;i++){
    //printf("%1.2f+j%1.2f ", senal[i].re, senal[i].im);
  //}
  //printf("\n\n");


  if (mylen>1){ 

    /* Cálculo de la etapa actual. */
    for(i=0;i<semi_len;i++){
      mariposa(&senal[i], &senal[i+semi_len], i*profundidad);
    }

    /* Cálculo de parámetros para asignación de tarea. */
    if (mylen <= (nro_muestras_total/np_total)){ /* Si la asginación ha llegado al límite. */
      /* No seguir repartiendo el trabajo. Tomarlo en el proceso actual. */
      posicion_segundo_hijo = posicion;  
      respons = myrank;
    }else{
      /* Repartir el trabajo. No tomarlo en el proceso actual. */
      posicion_segundo_hijo = posicion+(mylen>>1); /* Fijar posición de arranque en el vector. */
      respons = responsab(posicion_segundo_hijo);  /* Establecer proceso destinatario.         */
    }

    /* Asignación de tarea al segundo hijo. */
    if (respons==myrank){ /* Es que debe continuar calculando aún la segunda rama. */
      /* Es llamada la FFT a secas. */
      dft(senal+semi_len, semi_len,profundidad<<1,respons,posicion_segundo_hijo);    
    }else{
      //derivar_trabajo(senal+semi_n, semi_n, 2, responsab(semi_n), semi_n, 0);      
      derivar_trabajo(senal+semi_len, semi_len, profundidad<<1, respons, posicion_segundo_hijo, myrank);
    }

    dft(senal,   semi_len,profundidad<<1,myrank,posicion);
        
  }
}


void derivar_trabajo(scomplex* senal, uint semi_len, uint profundidad, uint respons, uint pos, uint myrank){
  /* Inicio comunicacion mediante MPI. */
  uint aux;
  //                 5               0        1                2                         3       4
  // DATOS PARA FFT: senal+semi_len  semi_len profundidad<<1   respons   posicion_segundo_hijo   //origen 
  printf("* %u.- Intentando derivar trabajo a %u...\n", myrank, respons);

  aux=semi_len;               MPI_Send(&aux, 1, MPI_UNSIGNED, respons, 123, MPI_COMM_WORLD); /* Longitud.     */
  aux=profundidad   ;         MPI_Send(&aux, 1, MPI_UNSIGNED, respons, 123, MPI_COMM_WORLD); /* Profundidad.  */
  aux=respons;                MPI_Send(&aux, 1, MPI_UNSIGNED, respons, 123, MPI_COMM_WORLD); /* Destino.      */
  aux=pos;                    MPI_Send(&aux, 1, MPI_UNSIGNED, respons, 123, MPI_COMM_WORLD); /* Posicion.     */     
  aux=myrank;                 MPI_Send(&aux, 1, MPI_UNSIGNED, respons, 123, MPI_COMM_WORLD); /* Origen.       */

  
  MPI_Send(senal, semi_len, complex_MPI, respons, 123, MPI_COMM_WORLD);
  /* Fin comunicación mediante MPI. */  

  //printf("* %u.- Derive con exito el trabajo a %u (verif=%5.9f).\n", myrank, respons,  calcular_suma_verif(semi_len, senal));
  printf("* %u.- Derive con exito el trabajo a %u.\n", myrank, respons);
}


float calcular_suma_verif(uint semi_len, scomplex* senal){
  uint aux;
  float verif=0;
  for(aux=0;aux<semi_len;aux++){
    verif+= (senal[aux].re + senal[aux].im);
  }
  return verif;
}



/* Algoritmo de bit-reverso. 
 * Es utilizado en el ordenamiento de los datos obtenidos en el proceeso.
 */
uint reversal_bit(uint a, uint n){
  uint i=0;               /* Índice de bucle.                                                     */
  uint base=0;            /* Base sobre la que se escriben los bits en orden inverso.             */
  uint mask = 1<<(n-1);   /* Máscara utilizada para analizar a la palabra original.               */
  uint shift=1;           /* Registro con bit en desplazamiento inverso a mask, para setear base. */

  for (i=0;i<n;i++){              /* n es la cantidad de bits necesarios para direccionar las muestras.   */
    base |= (mask & a)?shift:0;   /* La base es seteada según shift (<-), sólo si hay un 1 en la posición */
    mask = mask >> 1;             /* dada  por mask (->). */
    shift = shift << 1;
  }
  return base;
}


uint logbase2(uint arg){
  uint i=0, multip=1;
  while(multip<arg){
    multip<<=1;
    i++;
  }
  return i;
}

/* Establece la responsabilidad de cada etapa de la FFT.
 * Retorna el uP asociado a la posición del vector de muestras.
 */
uint responsab(uint posic){  
  uint myrank;
  uint bloque_por_uP = (uint)(nro_muestras_total/np_total);
  
  myrank = posic / bloque_por_uP;
  //printf("   Solicitada resp. (posic=%u -> resp=%u).\n",posic,myrank);
  return myrank;
}
