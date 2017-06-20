/* 
Trabalho Final de Circuitos Elétricos 2 - 2017.1

Emerson Santos
Juan Vazquez
Pedro Reis

Baseado no programa mna1amp
Por Antonio Carlos M. de Queiroz - acmq@coe.ufrj.br
http://www.coe.ufrj.br/~acmq/cursos/mna1amp.zip
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "macros.h"
#include "mna.h"
#include "netlist.h"
#include "resolver.h"
#include "tipos.h"

elemento netlist[MAX_ELEM]; /* Netlist */

int
  falhaLeitura, /* Status da leitura do netlist */
  falhaVariaveis, /* Status das variaveis de corrente */
  i,j,k;

char
/* Foram colocados limites nos formatos de leitura para alguma protecao
   contra excesso de caracteres nestas variaveis */
  nomearquivo[MAX_LINHA+1],
  tipo,
  na[MAX_NOME],nb[MAX_NOME],nc[MAX_NOME],nd[MAX_NOME],
  lista[MAX_NOS+1][MAX_NOME+2], /*Tem que caber jx antes do nome */
  txt[MAX_LINHA+1],
  *p;

FILE *arquivo;

double
  g,
  Yn[MAX_NOS+1][MAX_NOS+2],
  Yn1[MAX_NOS+1][MAX_NOS+2];

double _Complex
  YnPS[MAX_PONTOS+1][MAX_NOS+1][MAX_NOS+2];

contagem cont;

frequencia freq;
tabela C,L;

int main (int argc, char *argv[]) {
  do {
    printf("Entre nome do arquivo com o netlist (ex: mna.net): ");
    scanf("%50s", nomearquivo);

    arquivo = fopen(nomearquivo, "r");
  } while (arquivo == 0);

  for(i=0; i<=MAX_NOS; i++) {C[i]=i; L[i]=i;} /* Inicializa tabelas */
  cont.nv = 0; /* inicializa contagem de variáveis */
  cont.ne = 0;  /* inicializa contagem de elementos */
  freq.ptspor = 0; /* inicializa ptspor */
  strcpy(lista[0],"0"); /* inicializa nó 0 */

  falhaLeitura = lerNetlist(arquivo, netlist, txt, p, lista, &cont, &freq);
  fclose(arquivo);
  
  if (falhaLeitura) {
    exit(1);
  }

  falhaVariaveis = variaveisCorrente(&cont, lista, netlist, L, C);

  if (falhaVariaveis) {
    exit(1);
  }

  imprimirNetlist(&cont, lista, netlist, C);
  
  zerarMatrizesDouble(Yn, Yn1, &cont);
  
  k=0;
  do {
    for (i=0; i<=cont.neq; i++) {
      for (j=0; j<=cont.neq+1; j++) {
        Yn[i][j]=Yn1[i][j];
        Yn1[i][j]=0;
      }
    }

    mnaPO(netlist, Yn, Yn1, L, C, &cont);
    resolversistemaPO(Yn1, &cont);

    printf("Prévia do sistema\n");
    imprimeSistemaDouble(Yn1, &cont);
    printf("k=:%d\n",k);
    k++;
  } while (testeconvergenciaPO(Yn, Yn1, &cont) && (k<10));

  printf("Sistema resolvido:\n");
  imprimeSistemaDouble(Yn1, &cont);
 
  zerarMatrizDoubleComplex(YnPS, &cont, &freq);
         
  if (freq.ptspor==1) { /*Decada*/
    double f = freq.fi;
    double passo = pow(10, 1.0/freq.npts);
    
    printf("\nPasso : %lg \n",passo);
    k=0;
    do {
      for (i=0; i<=(freq.npts-1); i++) {
        mnaPS(netlist,Yn1,YnPS[k],L,C,f,&cont);
        resolversistemaPS(YnPS[k],&cont);

        printf("\nSistema resolvido PS na frequencia %lg :\n",f);
        imprimeSistemaDoubleComplex(YnPS[k], &cont, &freq);
        f=f*passo;
        k++;
      }
    } while(f<freq.fs);

    mnaPS(netlist,Yn1,YnPS[k],L,C,freq.fs,&cont);
    resolversistemaPS(YnPS[k],&cont);

    printf("\nSistema resolvido PS na frequencia %lg :\n",f);
    imprimeSistemaDoubleComplex(YnPS[k], &cont, &freq);
  }
  else if (freq.ptspor==2) { /*Oitava*/
    double f = freq.fi;
    double passo = pow(2, 1.0/freq.npts);
    
    printf("\nPasso : %lg \n",passo);
    k=0;
    do {
      for (i=0; i<=(freq.npts-1); i++) {
        mnaPS(netlist,Yn1,YnPS[k],L,C,f,&cont);
        resolversistemaPS(YnPS[k],&cont);

        printf("\nSistema resolvido PS na frequencia %lg :\n",f);
        imprimeSistemaDoubleComplex(YnPS[k], &cont, &freq);
        f=f*passo;
        k++;
      }
    } while(f<freq.fs);

    mnaPS(netlist,Yn1,YnPS[k],L,C,freq.fs,&cont);
    resolversistemaPS(YnPS[k],&cont);

    printf("\nSistema resolvido PS na frequencia %lg :\n",f);
    imprimeSistemaDoubleComplex(YnPS[k], &cont, &freq);
  }
  else if (freq.ptspor==3) { /*Linear*/
    double f = freq.fi;

    for (i=0; i<(freq.npts-1); i++) {
      mnaPS(netlist,Yn1,YnPS[i],L,C,f,&cont);
      resolversistemaPS(YnPS[i],&cont);
      printf("\nSistema resolvido PS na frequencia %lg :\n",f);
      imprimeSistemaDoubleComplex(YnPS[i], &cont, &freq);
      f+=(freq.fs-freq.fi)/(freq.npts-1);
    }

    mnaPS(netlist,Yn,YnPS[i],L,C,f,&cont);
    resolversistemaPS(YnPS[i],&cont);
    printf("\nSistema resolvido PS na frequencia %lg :\n",f);
    imprimeSistemaDoubleComplex(YnPS[i], &cont, &freq);
  }
  
  /*imprimirsistemaPS*/
  return OK;
}
