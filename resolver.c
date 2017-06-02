#include "tipos.h"
#include "resolver.h"
#include <math.h>
#include <complex.h>
#include <stdio.h>


void resolversistemaPO(double  Yn[MAX_NOS+1][MAX_NOS+2],int neq)
{
  int i,j,l, a;
  double  t, p;

  for (i=1; i<=neq; i++) {
    t=0.0;
    a=i;
    for (l=i; l<=neq; l++) {
      if (fabs(Yn[l][i])>fabs(t)) {
	a=l;
	t=Yn[l][i];
      }
    }
    if (i!=a) {
      for (l=1; l<=neq+1; l++) {
	p=Yn[i][l];
	Yn[i][l]=Yn[a][l];
	Yn[a][l]=p;
      }
    }
    if (fabs(t)<TOLG) {
      printf("Sistema singular\n");
      return 1;
    }
    for (j=neq+1; j>0; j--) {  /* Basta j>i em vez de j>0 */
      Yn[i][j] /= t;
      p=Yn[i][j];
      if (p!=0)
        for (l=1; l<=neq; l++) {  
	  if (l!=i)
	    Yn[l][j]-=Yn[l][i]*p;
        }
    }
  }
}

int testeconvergenciaPO(double  Yn[MAX_NOS+1][MAX_NOS+2], double  Yn1[MAX_NOS+1][MAX_NOS+2], int neq)
{
   int i,j,l, a;
   double  t, p;
   for (i=1; i<=neq; i++) {
     for (j=neq+1; j>0; j--) { 
       d=Yn[i][j]-Yn1[i][j];
       if (fabs(d)>CONV_CONST) return 1;
     }     
   }
   return 0;
}

void resolversistemaPS(double _Complex Yn[MAX_NOS+1][MAX_NOS+2], int neq)
{
  int i,j,l, a;
  double _Complex  t, p;

  for (i=1; i<=neq; i++) {
    t=0.0 +0.0*I;
    a=i;
    for (l=i; l<=neq; l++) {
      if (cabs(Yn[l][i])>cabs(t)) {
	a=l;
	t=Yn[l][i];
      }
    }
    if (i!=a) {
      for (l=1; l<=neq+1; l++) {
	p=Yn[i][l];
	Yn[i][l]=Yn[a][l];
	Yn[a][l]=p;
      }
    }
    if (cabs(t)<TOLG) {
      printf("Sistema singular\n");
      return 1;
    }
    for (j=neq+1; j>0; j--) {  /* Basta j>i em vez de j>0 */
      Yn[i][j] /= t;
      p=Yn[i][j];
      if (p!=0)
        for (l=1; l<=neq; l++) {  
	  if (l!=i)
	    Yn[l][j]-=Yn[l][i]*p;
        }
    }
  }
}
