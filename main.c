﻿/* 
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
#include <complex.h>
#include <time.h>

#include "macros.h"
#include "mna.h"
#include "netlist.h"
#include "resolver.h"
#include "tipos.h"


elemento netlist[MAX_ELEM]; /* Netlist */

int
  falhaLeitura, /* Status da leitura do netlist */
  falhaVariaveis, /* Status das variaveis de corrente */
  i,j,k,n,
  nPontos; /* Guarda número de pontos analisados */

char
/* Foram colocados limites nos formatos de leitura para alguma protecao
   contra excesso de caracteres nestas variaveis */
  nomearquivo[MAX_LINHA+1],
  fimDoPrograma[MAX_LINHA+1],
  tipo,
  na[MAX_NOME],nb[MAX_NOME],nc[MAX_NOME],nd[MAX_NOME],
  lista[MAX_NOS+1][MAX_NOME+2], /*Tem que caber jx antes do nome */
  txt[MAX_LINHA+1],
  *p;

FILE *arquivo;
FILE *arquivoTab;

double
  g,sorte,
  Yn[MAX_NOS+1][MAX_NOS+2],
  Yn1[MAX_NOS+1][MAX_NOS+2];

double _Complex
  YnPS[MAX_PONTOS+1][MAX_NOS+1][MAX_NOS+2];

contagem cont;

frequencia freq;
tabela C,L;

char *nomeTab, linhaDoArquivo[1024], auxiliar[100];


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
    printf("Falha de leitura.\nDigite qualquer tecla e aperte enter para encerrar o programa\n");
    scanf("%50s", fimDoPrograma);
    return OK;
  }
  falhaVariaveis = variaveisCorrente(&cont, lista, netlist, L, C);

  if (falhaVariaveis) {
    printf("Falha de variaveis.\nDigite qualquer tecla e aperte enter para encerrar o programa\n");
    scanf("%50s", fimDoPrograma);
    return OK;
  }

  imprimirNetlist(&cont, lista, netlist, C);
  
  zerarMatrizesDouble(Yn, Yn1, &cont);
  srand((unsigned)time(NULL));
  n=1;
  do {
    k=0;
    do {
      for (i=0; i<=cont.neq; i++) {
        for (j=0; j<=cont.neq+1; j++) {
          if (i>0) Yn[i][j]=Yn1[i][j];
          else Yn[i][j]=0;
          Yn1[i][j]=0;
        }
      }

      mnaPO(netlist, Yn, Yn1, L, C, &cont);
      if(resolversistemaPO(Yn1, &cont)) k=49;

      //printf("Prévia do sistema\n");
      //imprimeSistemaDouble(Yn1, &cont);
      //printf("k=:%d n=:%d\n",k,n);
      k++;
    } while (testeconvergenciaPO(Yn, Yn1, &cont) && (k<50));
    if (k==50){
      //zerarMatrizesDouble(Yn, Yn1, &cont);
      for (i=0; i<=cont.neq; i++) {
        double d=Yn[i][cont.neq+1]-Yn1[i][cont.neq+1];
        if (fabs(d)>1) d=d/fabs(Yn[i][j]);
        if (fabs(d)>CONST_CONV) {
          sorte=((double)(rand()%100))/10;
          Yn1[i][cont.neq+1]=sorte;
        }
      }
      //printf("Prévia do sistema\n");
      //imprimeSistemaDouble(Yn1, &cont);
      //printf("k=:%d n=:%d\n",k,n);
      n++;
    }
  } while ((k==50) && (n<50));

  if (testeconvergenciaPO(Yn, Yn1, &cont)) {
    printf("\nSolucao:\n");
    strcpy(txt,"Tensao");
    for (i=1; i<=cont.nv; i++) {
      if (i==cont.nn+1) strcpy(txt,"Corrente");
      if (C[i]!=0)
        printf("%s %s (%d): %g\n",txt,lista[i],C[i],Yn[C[i]][cont.neq+1]);
      else
        printf("%s %s (%d): nao calculada\n",txt,lista[i],C[i]);
    }

    printf("Nao convergiu.\nDigite qualquer tecla e aperte enter para encerrar o programa\n");
    scanf("%50s", fimDoPrograma);
    return OK;
  }

  //printf("Sistema resolvido:\n");
  //imprimeSistemaDouble(Yn1, &cont);

  printf("\nSolucao:\n");
  strcpy(txt,"Tensao");
  for (i=1; i<=cont.nv; i++) {
    if (i==cont.nn+1) strcpy(txt,"Corrente");
    if (C[i]!=0)
      printf("%s %s (%d): %g\n",txt,lista[i],C[i],Yn[C[i]][cont.neq+1]);
    else
      printf("%s %s (%d): nao calculada\n",txt,lista[i],C[i]);
  }

  for (int y = 1; y <= cont.neq+1; y++) {
    Yn1[0][y] = 0;
  }
 
  zerarMatrizDoubleComplex(YnPS, &cont, &freq);

  if (freq.ptspor != 0) {
    /* Prepara tabela de saída */
    size_t caracteres = strlen(nomearquivo);

    nomeTab = (char *) malloc((caracteres+2) * sizeof(char));
    nomeTab[0] = '\0';
    linhaDoArquivo[0] = '\0';
    auxiliar[0] = '\0';

    strncat(nomeTab, nomearquivo, (caracteres-4));
    strcat(nomeTab, ".tab\0");

    arquivoTab = fopen(nomeTab, "w");

    strcat(linhaDoArquivo, "f ");

    for (int i=1; i<=cont.neq; i++) {
        strcat(linhaDoArquivo, lista[i]);
        strcat(linhaDoArquivo, "m ");
        strcat(linhaDoArquivo, lista[i]);
        strcat(linhaDoArquivo, "f ");
    }

    strcat(linhaDoArquivo, "\n");
    fprintf(arquivoTab, linhaDoArquivo);

    /* Começa a calcular PS */

    double f = freq.fi;

    k = 0;
    do {
      mnaPS(netlist,Yn1,YnPS[k],L,C,f,&cont);
      resolversistemaPS(YnPS[k],&cont);

      linhaDoArquivo[0] = '\0';
      snprintf(auxiliar, 20, "%3.6f", f);
      strcat(linhaDoArquivo, auxiliar);
      strcat(linhaDoArquivo, " ");
      
      for (int coluna = 1; coluna <= cont.neq; coluna+=1) {
          snprintf(auxiliar, 20, "%3.7f", (cabs(YnPS[k][coluna][cont.neq+1])) );
          strcat(linhaDoArquivo, auxiliar);
          strcat(linhaDoArquivo, " ");
          snprintf(auxiliar, 20, "%3.5f", graus(carg(YnPS[k][coluna][cont.neq+1])));
          strcat(linhaDoArquivo, auxiliar);
          strcat(linhaDoArquivo, " ");
      }

      strcat(linhaDoArquivo, "\n");
      fprintf(arquivoTab, linhaDoArquivo);

      if (freq.ptspor == 1) {
        f *= pow(10, 1.0/(freq.npts-1));
      } 
      else if (freq.ptspor == 2) {
        f *= pow(2, 1.0/(freq.npts-1));
      }
      else if (freq.ptspor == 3) {
        f += (freq.fs-freq.fi)/(freq.npts-1);
      }

      k++;
    } while (f < freq.fs);
    
    mnaPS(netlist,Yn1,YnPS[k],L,C,freq.fs,&cont);
    resolversistemaPS(YnPS[k],&cont);

    linhaDoArquivo[0] = '\0';
    snprintf(auxiliar, 20, "%3.2f", freq.fs);
    strcat(linhaDoArquivo, auxiliar);
    strcat(linhaDoArquivo, " ");
    
    for (int coluna = 1; coluna <= cont.neq; coluna+=1) {
        snprintf(auxiliar, 20, "%3.5f", (cabs(YnPS[k][coluna][cont.neq+1])));
        strcat(linhaDoArquivo, auxiliar);
        strcat(linhaDoArquivo, " ");
        snprintf(auxiliar, 20, "%3.5f", graus(carg(YnPS[k][coluna][cont.neq+1])));
        strcat(linhaDoArquivo, auxiliar);
        strcat(linhaDoArquivo, " ");
    }

    strcat(linhaDoArquivo, "\n");
    fprintf(arquivoTab, linhaDoArquivo);

    fclose(arquivoTab);
    
    
    printf("\nArquivo .tab da resposta em frequencia foi salvo com o mesmo nome do netlist.\n");
  }
  
  printf("Programa concluido.\nDigite qualquer tecla e aperte enter para encerrar o programa\n");
  scanf("%50s", fimDoPrograma);
  return OK;
}
