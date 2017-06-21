#include <complex.h>
#include <math.h>
#include <stdio.h>

#include "resolver.h"
#include "tipos.h"
#include "macros.h"


int resolversistemaPO(double  Yn[MAX_NOS+1][MAX_NOS+2],contagem *cont) {
  int i,j,l, a;
  double  t, p;

  for (i=1; i<=cont->neq; i++) {
    t=0.0;
    a=i;
    
    for (l=i; l<=cont->neq; l++) {
      if (fabs(Yn[l][i])>fabs(t)) {
        a=l;
        t=Yn[l][i];
      }
    }

    if (i!=a) {
      for (l=1; l<=cont->neq+1; l++) {
        p=Yn[i][l];
        Yn[i][l]=Yn[a][l];
        Yn[a][l]=p;
      }
    }

    if (fabs(t)<TOLG) {
      printf("Sistema singular\n");
      return 1;
    }
    
    for (j=cont->neq+1; j>0; j--) {  /* Basta j>i em vez de j>0 */
      Yn[i][j] /= t;
      p=Yn[i][j];

      if (p!=0) {
        for (l=1; l<=cont->neq; l++) {  
          if (l!=i) Yn[l][j]-=Yn[l][i]*p;
        }
      }        
    }
  }

  return OK;
}

int testeconvergenciaPO(double  Yn[MAX_NOS+1][MAX_NOS+2], double  Yn1[MAX_NOS+1][MAX_NOS+2], contagem *cont) {
   int i,j;
   double d;
   
   for (i=1; i<=cont->neq; i++) {
     for (j=cont->neq+1; j>0; j--) { 
       d=Yn[i][j]-Yn1[i][j];
       if (fabs(d)>1) d=d/fabs(Yn[i][j]);
       if (fabs(d)>CONST_CONV) return 1;
     }     
   }

   return OK;
}

int resolversistemaPS(double _Complex Yn[MAX_NOS+1][MAX_NOS+2], contagem *cont) {
  int i,j,l, a;
  double _Complex  t, p;

  for (i=1; i<=cont->neq; i++) {
    t=0.0 +0.0*I;
    a=i;

    for (l=i; l<=cont->neq; l++) {
      if (cabs(Yn[l][i])>cabs(t)) {
        a=l;
        t=Yn[l][i];
      }
    }

    if (i!=a) {
      for (l=1; l<=cont->neq+1; l++) {
        p=Yn[i][l];
        Yn[i][l]=Yn[a][l];
        Yn[a][l]=p;
      }
    }

    if (cabs(t)<TOLG) {
      printf("Sistema singular\n");
      return 1;
    }

    for (j=cont->neq+1; j>0; j--) {  /* Basta j>i em vez de j>0 */
      Yn[i][j] /= t;
      p=Yn[i][j];

      if (p!=0) {
        for (l=1; l<=cont->neq; l++) {  
          if (l!=i) Yn[l][j]-=Yn[l][i]*p;
        }
      }
    }
  }

  return OK;
}

void imprimeSistemaDouble(double Yn[MAX_NOS+1][MAX_NOS+2], contagem *cont) {
  for (int i=1; i<=cont->neq; i++) {
    for (int j=1; j<=cont->neq+1; j++)
      if (Yn[i][j]!=0) printf("%+3.1f ",Yn[i][j]);
      else printf(" ... ");
    printf("\n");
  }
}

void zerarMatrizesDouble(double Yn[MAX_NOS+1][MAX_NOS+2], double Yn1[MAX_NOS+1][MAX_NOS+2], contagem *cont) {
  int i, j;

  for (i=0; i<=cont->neq; i++) {
    for (j=0; j<=cont->neq+1; j++) {
      Yn[i][j]=0;
      Yn1[i][j]=0;
    }
  }
}

void zerarMatrizDoubleComplex(double _Complex YnPS[MAX_PONTOS+1][MAX_NOS+1][MAX_NOS+2], contagem *cont, frequencia *freq) {
  int i, j, k;

  for (i=0; i<=cont->neq; i++) {
    for (j=0; j<=cont->neq+1; j++) {
      for (k=0; k<=freq->npts; k++) {
        YnPS[k][i][j]=0;
      }
    }
  }
}

void imprimeSistemaDoubleComplex(double _Complex YnPS[MAX_NOS+1][MAX_NOS+2], contagem *cont) {
  for (int i=1; i<=cont->neq; i++) {
    for (int j=1; j<=cont->neq+1; j++) {
      if (YnPS[i][j]!=0 && cimag(YnPS[i][j]) == 0) printf("%+3.3f ", creal(YnPS[i][j]));
      else if (YnPS[i][j]!=0 && cimag(YnPS[i][j]) != 0) printf("%+3.3f <%+3.1fº", creal(YnPS[i][j]), graus(carg(YnPS[i][j])));
      else printf(" ..... ");
    }
    printf("\n");
  }
}

double graus(double radianos) {
    return radianos * (180.0 / M_PI);
}
