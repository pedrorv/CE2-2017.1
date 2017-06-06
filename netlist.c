#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "macros.h"
#include "netlist.h"
#include "tipos.h"

/* Rotina que conta os nos e atribui numeros a eles */
int numero(char *nome, char lista[MAX_NOS+1][MAX_NOME+2]) {
  int i,achou,nv;

  i=0; achou=0; nv=0;
  
  while (!achou && i<=nv)
    if (!(achou=!strcmp(nome,lista[i]))) i++;
  
  if (!achou) {
    if (nv==MAX_NOS) {
      printf("O programa so aceita ate %d nos\n",nv);
      exit(1);
    }
    nv++;
    strcpy(lista[nv],nome);
    return nv; /* novo no */
  }
  else {
    return i; /* no ja conhecido */
  }
}

int lerNetlist(FILE *arquivo, elemento netlist[MAX_ELEM], char txt[MAX_LINHA+1], char *p, char lista[MAX_NOS+1][MAX_NOME+2]) {
    char tipo, na[MAX_NOME],nb[MAX_NOME],nc[MAX_NOME],nd[MAX_NOME];
    int ne = 0;

    printf("Lendo netlist:\n");
    fgets(txt, MAX_LINHA, arquivo);
    printf("Título: %s", txt);

    while (fgets(txt, MAX_LINHA, arquivo)) {
        ne++; /* Não usa o netlist[0] */
        if (ne > MAX_ELEM) {
            printf("O programa só aceita até %d elementos.\n", MAX_ELEM);
            return 1;
        }

        txt[0] = toupper(txt[0]);
        tipo = txt[0];

        sscanf(txt, "%10s", netlist[ne].nome);
        p = txt + strlen(netlist[ne].nome);

        if (tipo == 'R' || tipo == 'I' || tipo == 'V') {
            sscanf(p,"%10s%10s%lg",na,nb,&netlist[ne].valor);
            printf("%s %s %s %g\n",netlist[ne].nome,na,nb,netlist[ne].valor);
            netlist[ne].a=numero(na, lista);
            netlist[ne].b=numero(nb, lista);
        }

        else if (tipo=='G' || tipo=='E' || tipo=='F' || tipo=='H') {
            sscanf(p,"%10s%10s%10s%10s%lg",na,nb,nc,nd,&netlist[ne].valor);
            printf("%s %s %s %s %s %g\n",netlist[ne].nome,na,nb,nc,nd,netlist[ne].valor);
            netlist[ne].a=numero(na, lista);
            netlist[ne].b=numero(nb, lista);
            netlist[ne].c=numero(nc, lista);
            netlist[ne].d=numero(nd, lista);
        }
        else if (tipo=='O') {
            sscanf(p,"%10s%10s%10s%10s",na,nb,nc,nd);
            printf("%s %s %s %s %s\n",netlist[ne].nome,na,nb,nc,nd);
            netlist[ne].a=numero(na, lista);
            netlist[ne].b=numero(nb, lista);
            netlist[ne].c=numero(nc, lista);
            netlist[ne].d=numero(nd, lista);
        }
        else if (tipo=='*') { /* Comentario comeca com "*" */
            printf("Comentario: %s", txt);
            ne--;
        }
        else {
            printf("Elemento desconhecido: %s\n", txt);
            return 1;
        }
    }

    return OK;
}