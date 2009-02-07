#include <stdio.h>
#include <stdlib.h>
#include "complejo.h"
#include "fft.h"
#include <mpi.h>
#define ROOT 0

#define MPI_TAGMUE 123
#define MPI_TAGPARAM 757

/* Variables globales. No son modificadas luego de su asinación inicial. 
 * No representan problemas de concurrencia. 
 */
uint nro_muestras_total;  /* Cantidad de muestras a ser procesadas. Cumple nro=2^k (k entero).  */
uint np_total;            /* Cantidad de procesadores a ser utilizados.                         */
uint myrank;

MPI_Datatype complex_MPI;
scomplex *twiddle_factors;


void fft(scomplex *senal,scomplex *fft_res, uint n,uint nproc){
  uint  i, semi_n;
  float arg;

  MPI_Bcast(&n, 1, MPI_UNSIGNED, ROOT, MPI_COMM_WORLD); /* ROOT indica el tamaño del vector. */

  if ((2*n)<nproc){
    printf("Error. Debe haber al menos 2 muestras por procesador.\n"); MPI_Finalize(); exit(-1);
  }
  semi_n=n>>1;
  nro_muestras_total=n;
  np_total = nproc;

  /* Cálculo de los factores de fase. */
  twiddle_factors = (scomplex*) malloc((n>>1)*sizeof(scomplex));
  for (i=0;i<(n>>1);i++){
    arg = PI2*i/n;
	  twiddle_factors[i].re = cos((double)arg);
	  twiddle_factors[i].im = sin((double)arg);
  }

  /* Creación de un nuevo tipo MPI. 1 Complejo. */
  MPI_Type_contiguous(2, MPI_DOUBLE, &complex_MPI); 
  MPI_Type_commit(&complex_MPI);
  MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

  if (myrank == ROOT){
    /**** Proceso ROOT. ****/
    printf("* %u.- Calculando primera etapa...\n",myrank);
    for(i=0;i<semi_n;i++){ 
      mariposa(&senal[i], &senal[i+semi_n], i);
    }  

    if (nproc!=1){
      //printf("* %u.- Listo. Enviando mitad procesada a %u (%u elementos)...\n",myrank,responsab(semi_n), semi_n);
      derivar_trabajo(senal+semi_n, semi_n, 2, responsab(semi_n), semi_n, 0);
    } 

    printf("* %u.- Primera llamada a FFT...\n",myrank);
    dft(senal, semi_n, 2, 0, 0);  
    
    if (nproc!=1){  /* Caso en el que no se encuentra sólo el ROOT, sino con otros uP. */
      free(twiddle_factors);
      uint bloque = nro_muestras_total/np_total;
      printf("* %u.- FFT parcial lista. Esperando vectores (%u elementos c/u)...\n",myrank, bloque);
      MPI_Gather(senal, bloque, complex_MPI, senal, bloque, complex_MPI, ROOT, MPI_COMM_WORLD);

      printf("* %u.- Vectores parciales recibidos. Reenviando vector agrupado para ordenar...\n",myrank);
      MPI_Bcast(senal, n, complex_MPI, ROOT, MPI_COMM_WORLD);

      printf("* %u.- Vector desordenado ya enviado. Ordenando...\n", myrank);
      scomplex* fft_parc = (scomplex*) malloc((n/np_total)*sizeof(scomplex));
      ordenar_bit_reversal_parcial(senal, fft_parc, n, np_total, myrank);
      printf("* %u.- Vector parcial ordenado. Esperando demas vectores...\n",myrank);
      MPI_Gather(fft_parc, bloque, complex_MPI, fft_res, bloque, complex_MPI, ROOT, MPI_COMM_WORLD);  
      printf("* %u.- Vectores recibidos y agrupados. Orden listo.\n",myrank);
    }else{ /* El ROOT es el único uP. */
      dft(senal+semi_n, semi_n, 2, 0, semi_n);  
      free(twiddle_factors);
      printf("* %u.- Calculada FFT. Ordenando...\n",myrank);
      ordenar_bit_reversal_parcial(senal, fft_res, n, 1, 0);
      printf("* %u.- Orden listo.\n",myrank);
    }
  }else{
    /**** Procesos NO ROOT. ****/
    fft_recibir_tarea();
    printf("* %u.- Esperando datos para ordenar...\n", myrank);
    scomplex* senal = (scomplex*) malloc(n*sizeof(scomplex));          
    MPI_Bcast(senal, n, complex_MPI, ROOT, MPI_COMM_WORLD);
    printf("* %u.- Datos recibidos. Ordenando...\n", myrank);
    scomplex* fft_parc = (scomplex*) malloc((n/np_total)*sizeof(scomplex));
    ordenar_bit_reversal_parcial(senal, fft_parc, n, np_total, myrank);
    free(senal);
    uint bloque = nro_muestras_total/np_total;
    printf("* %u.- Vector parcial ordenado. Enviando...\n", myrank);
    MPI_Gather(fft_parc, bloque , complex_MPI, NULL, 0, complex_MPI, ROOT, MPI_COMM_WORLD);
    printf("* %u.- Enviado el vector parcial con exito.\n", myrank);
    free(fft_parc);
  }
}


/* Ordena parcialmente un vector complejo
 * en orden reverso. 
 */
void ordenar_bit_reversal_parcial(scomplex* senal, scomplex* fft_res, uint n, uint np, uint myrank){
  scomplex interm;
  uint len_bits, aux, i,j;
  len_bits = logbase2(n);
  for(i=responsab_ord(myrank),j=0;j < (uint)(nro_muestras_total/np_total);i++,j++){ 
    aux = reversal_bit(i,len_bits);
    fft_res[j].re = senal[aux].re;
    fft_res[j].im = senal[aux].im;  
  }
}

/* Función encargada de detener un proceso a la espera de
 * una tarea determinada. Recibe parámetros y muestras para
 * trabajar. 
 */
void fft_recibir_tarea(){
  MPI_Status status;
  uint aux, len, prof, dest, pos, orig, buf[5];  
  scomplex *vector_aux;

  printf("* %u.- Espero a recibir tarea...\n", myrank);
  MPI_Recv(buf, 5, MPI_UNSIGNED, MPI_ANY_SOURCE, MPI_TAGPARAM, MPI_COMM_WORLD, &status); /* Buffer de parámetros. */
  len=buf[0];prof=buf[1];dest=buf[2];pos=buf[3];orig=buf[4];

  printf("* %u.- Recibida tarea (orig=%u prof=%u dest=%u pos=%u).\n", myrank,orig,prof,dest,pos);
  vector_aux = (scomplex*) malloc(len*sizeof(scomplex));
  MPI_Recv(vector_aux, len, complex_MPI, MPI_ANY_SOURCE, MPI_TAGMUE, MPI_COMM_WORLD, &status);  /* Origen.       */

  
  //printf("* %u.- Recibi datos con exito (verif=%f). Calculando FFT...\n", myrank,(double)calcular_suma_verif(len, vector_aux));
  printf("* %u.- Recibi datos con exito. Calculando FFT...\n", myrank);

  dft(vector_aux, len,prof,myrank,pos);
  printf("* %u.- Calcule FFT con exito. Enviando a ROOT %u elementos...\n", myrank, nro_muestras_total/np_total);
  uint bloque = nro_muestras_total/np_total;
  MPI_Gather(vector_aux, bloque , complex_MPI, NULL, 0, complex_MPI, ROOT, MPI_COMM_WORLD);
  free(vector_aux);
  printf("* %u.- Envie resultado a ROOT con exito.\n", myrank);
 
}


/* Función de FFT con recursividad.
 * Establece las responsabilidades para cada uP.
 */
void dft(scomplex *senal, uint mylen, uint profundidad, uint myrank, uint posicion){
  uint i, semi_len=mylen>>1, posicion_segundo_hijo, responsable;
  
  if (mylen>1){ /* Condición de corte de la recursividad. */
    for(i=0;i<semi_len;i++){ /* Cálculo de la etapa actual. */
      mariposa(&senal[i], &senal[i+semi_len], i*profundidad);
    }

    /* Cálculo de parámetros para asignación de tarea. */
    if (mylen <= (nro_muestras_total/np_total)){ /* Si la asginación ha llegado al límite... */
      posicion_segundo_hijo = posicion; /* No seguir repartiendo el trabajo. */
      responsable = myrank;             /* Tomarlo en el proceso actual.     */
    }else{ /* O, repartir el trabajo. No tomarlo en el proceso actual. */
      posicion_segundo_hijo = posicion+(mylen>>1);    /* Fijar posición de arranque en el vector. */
      responsable = responsab(posicion_segundo_hijo); /* Establecer proceso destinatario.         */
    }

    /* Asignación de tarea al segundo hijo. */
    if (responsable==myrank){ /* Debe continuar calculando aún la segunda rama... */
      dft(senal+semi_len, semi_len,profundidad<<1,responsable,posicion_segundo_hijo); /* Es llamada la FFT a secas. */    
    }else{                /* O, deriva el trabajo hacia el hijo.               */
      derivar_trabajo(senal+semi_len, semi_len, profundidad<<1, responsable, posicion_segundo_hijo, myrank);
    }

    dft(senal,   semi_len,profundidad<<1,myrank,posicion);  /* Calcula la primera rama siempre. */
  }
}

/* Realiza la derivación de tareas de
 * la FFT, luego de una bifurcación.
 */
void derivar_trabajo(scomplex* senal, uint semi_len, uint profundidad, uint respons, uint pos, uint myrank){
  uint buf[5];  /* Buffer donde enviar los parámetros de la DFT remota. */
  printf("* %u.- Intentando derivar trabajo a %u...\n", myrank, respons);

  /* Comunicación mediante MPI. */
  buf[0]=semi_len; buf[1]=profundidad; buf[2]=respons; buf[3]=pos; buf[4]=myrank;
  MPI_Send(buf, 5, MPI_UNSIGNED, respons, MPI_TAGPARAM, MPI_COMM_WORLD);        /* Buffer de parámetros.  */
  MPI_Send(senal, semi_len, complex_MPI, respons, MPI_TAGMUE, MPI_COMM_WORLD);  /* Vector.                */
  
  printf("* %u.- Derive con exito (orig=%u prof=%u dest=%u pos=%u).\n", myrank,myrank,profundidad,respons,pos);
  //printf("* %u.- Derive con exito el trabajo a %u (verif=%f).\n", myrank, respons, (double)calcular_suma_verif(semi_len, senal));
}



