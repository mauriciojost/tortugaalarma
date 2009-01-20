/*	Fft.c
	
	1-d fast Fourier transform subroutine.
	From Claerbout (1985) p. 70.

	Usage:		int lx;
			float signi, scale;
			struct complex cx[];

			fft(lx, cx, signi, scale);

	Compile:	cc prog.c fft.c -fswitch -lm -o prog.c

	Arguments:	lx	number of elements of vector to transform
				MUST BE A POWER OF 2
			cx	pointer to vector of complex structures
			signi	sign of transform- +1 forward to Fourier domain
						   -1 inverse to real domain
			scale	scale factor to apply to data before tranform
									*/
/* Include the usual header files */
#include <stdio.h>
#include <math.h>

/* Define complex multiplication and its conjugate */
#define  rmul(x,y)      (x.re * y.re - x.im * y.im)
#define  imul(x,y)      (x.im * y.re + x.re * y.im)
#define rcmul(x,y)      (x.re * y.re + x.im * y.im)
#define icmul(x,y)      (x.im * y.re - x.re * y.im)

/* Declare the structure to hold complex numbers */
struct complex 
	{
	double re; 
	double im;
	};

/* Body of subroutine */
fft(lx, cx, signi, sc)
/* Declare all argument variables */
int lx;
float signi, sc;
struct complex *cx;
	{
	/* Declare all local variables */
	int i, j, k, m, istep;
	float arg=3;
	struct complex cw, ct;

  
	j = 0;
	k = 1;
	/* Left out for compatibility with colmft.c
	sc = (float)(sqrt(1.0/(double)(lx)));
	*/

  /* Aparentemente es el orden reverso de los elementos del vector de muestras. */
	for(i=0; i<lx; i++){          /* Procesa desde el primero hasta el último.    */

    /* Este if hace (como resumen un swap):       cx[j] <=> cx[i]               */
		if (i <= j){
			ct.re = sc * cx[j].re;    /* ct = sc*cx[j]. Backup de cx[j].              */
			ct.im = sc * cx[j].im;    /*                                              */
			cx[j].re = sc * cx[i].re; /* cx[j]=sc*cx[i].                              */
			cx[j].im = sc * cx[i].im; /*                                              */
			cx[i].re = ct.re;         /* cx[i]=ct.                                    */
			cx[i].im = ct.im;         /*                                              */
		}

		m = lx/2;

		while (j > m-1){
			j = j - m;
			m = m/2;
			if (m < 1)
				break;
		}
		j = j + m;

	}



	do{
		istep = 2*k;
		for (m=0; m<k; m++){
			arg = 3.14159265*signi*m/k;
			cw.re = cos((double)arg);
			cw.im = sin((double)arg);
			for (i=m; i<lx; i+=istep){
				ct.re = rmul(cw, cx[i+k]);    /*             ct   =  cw   * cx[i+k]     */
				ct.im = imul(cw, cx[i+k]);
				cx[i+k].re = cx[i].re - ct.re;/*          cx[i+k] = cx[i] -   ct        */
				cx[i+k].im = cx[i].im - ct.im;
				cx[i].re = cx[i].re + ct.re;  /*           cx[i]  = cx[i] +   ct        */
				cx[i].im = cx[i].im + ct.im;
			}
		}
		k = istep;
	}while (k < lx);

	return(0);
}

