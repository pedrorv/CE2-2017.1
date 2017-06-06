#include "macros.h"
#include "tipos.h"

int numero(char *nome, char lista[MAX_NOS+1][MAX_NOME+2], int nv);
int lerNetlist(FILE *arquivo, elemento netlist[MAX_ELEM], char txt[MAX_LINHA+1], char *p, char lista[MAX_NOS+1][MAX_NOME+2], int nv);
int variaveisCorrente(int ne, int nn, int nv, char lista[MAX_NOS+1][MAX_NOME+2], elemento netlist[MAX_ELEM]);