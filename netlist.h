#include "macros.h"
#include "tipos.h"

int numero(char *nome, char lista[MAX_NOS+1][MAX_NOME+2], contagem *cont);
int lerNetlist(FILE *arquivo, elemento netlist[MAX_ELEM], char txt[MAX_LINHA+1], char *p, char lista[MAX_NOS+1][MAX_NOME+2], contagem *cont, frequencia *freq);
int variaveisCorrente(contagem *cont, char lista[MAX_NOS+1][MAX_NOME+2], elemento netlist[MAX_ELEM], tabela L, tabela C);
void imprimirNetlist(contagem *cont, char lista[MAX_NOS+1][MAX_NOME+2], elemento netlist[MAX_ELEM], tabela C);