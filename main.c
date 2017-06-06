/* 
Trabalho Final de Circuitos Elétricos 2 - 2017.1

Emerson Santos
Juan Vazquez
Pedro Reis
*/

#include <stdio.h>
#include <stdlib.h>

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
  Yn[MAX_NOS+1][MAX_NOS+2];

contagem cont;

int main (int argc, char *argv[]) {  
  do {
    printf("Entre nome do arquivo com o netlist (ex: mna.net): ");
    scanf("%50s", nomearquivo);

    arquivo = fopen(nomearquivo, "r");
  } while (arquivo == 0);

  cont.nv = 0;
  cont.ne = 0;

  falhaLeitura = lerNetlist(arquivo, netlist, txt, p, lista, &cont);
  fclose(arquivo);
  
  if (falhaLeitura) {
    exit(1);
  }

  falhaVariaveis = variaveisCorrente(&cont, lista, netlist);

  if (falhaVariaveis) {
    exit(1);
  }

  imprimirNetlist(&cont, lista, netlist);

  return OK;
}