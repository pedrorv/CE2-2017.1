#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "macros.h"
#include "mna.h"
#include "netlist.h"
#include "tipos.h"

/* Rotina que conta os nos e atribui numeros a eles */
int numero(char *nome, char lista[MAX_NOS+1][MAX_NOME+2], contagem *cont) {
  int i,achou;

  i=0; achou=0;
  
  while (!achou && i<=cont->nv)
    if (!(achou=!strcmp(nome,lista[i]))) i++;
  
  if (!achou) {
    if (cont->nv==MAX_NOS) {
      printf("O programa so aceita ate %d nos\n",cont->nv);
      exit(1);
    }
    cont->nv++;
    strcpy(lista[cont->nv],nome);
    return cont->nv; /* novo no */
  }
  else {
    return i; /* no ja conhecido */
  }
}

int testarnos(contagem *cont) {
 if (cont->nv>MAX_NOS) {
   printf("As variaveis extra excederam o numero de variaveis permitido (%d)\n",MAX_NOS);
   return 1;
 }

 return OK;
}

int lerNetlist(FILE *arquivo, elemento netlist[MAX_ELEM], char txt[MAX_LINHA+1], char *p, char lista[MAX_NOS+1][MAX_NOME+2], contagem *cont, frequencia *freq) {
    char tipo, na[MAX_NOME],nb[MAX_NOME],nc[MAX_NOME],nd[MAX_NOME],ptspor[MAX_NOME];

    printf("Lendo netlist:\n");
    fgets(txt, MAX_LINHA, arquivo);
    printf("Título: %s", txt);

    while (fgets(txt, MAX_LINHA, arquivo)) {
        cont->ne++; /* Não usa o netlist[0] */
        if (cont->ne > MAX_ELEM) {
            printf("O programa só aceita até %d elementos.\n", MAX_ELEM);
            return 1;
        }

        txt[0] = toupper(txt[0]);
        tipo = txt[0];

        sscanf(txt, "%10s", netlist[cont->ne].nome);
        p = txt + strlen(netlist[cont->ne].nome);

        if (tipo == 'R' || tipo == 'L' || tipo == 'C') {
            sscanf(p,"%10s%10s%lg",na,nb,&netlist[cont->ne].valor);
            printf("%s %s %s %g\n",netlist[cont->ne].nome,na,nb,netlist[cont->ne].valor);
            netlist[cont->ne].a=numero(na, lista, cont);
            netlist[cont->ne].b=numero(nb, lista, cont);
        }
        else if (tipo=='K') {
          sscanf(p,"%10s%10s%lg",netlist[cont->ne].la,netlist[cont->ne].lb,&netlist[cont->ne].valor);
          printf("%s %s %s %g\n",netlist[cont->ne].nome,netlist[cont->ne].la,netlist[cont->ne].lb,netlist[cont->ne].valor);
        }
        else if (tipo == 'I' || tipo == 'V') {
            sscanf(p,"%10s%10s%lg%lg%lg",na,nb,&netlist[cont->ne].modulo,&netlist[cont->ne].fase,&netlist[cont->ne].valor);
            printf("%s %s %s %g %g %g\n",netlist[cont->ne].nome,na,nb,netlist[cont->ne].modulo,netlist[cont->ne].fase,netlist[cont->ne].valor);
            netlist[cont->ne].a=numero(na, lista, cont);
            netlist[cont->ne].b=numero(nb, lista, cont);
        }
        else if (tipo=='G' || tipo=='E' || tipo=='F' || tipo=='H') {
            sscanf(p,"%10s%10s%10s%10s%lg",na,nb,nc,nd,&netlist[cont->ne].valor);
            printf("%s %s %s %s %s %g\n",netlist[cont->ne].nome,na,nb,nc,nd,netlist[cont->ne].valor);
            netlist[cont->ne].a=numero(na, lista, cont);
            netlist[cont->ne].b=numero(nb, lista, cont);
            netlist[cont->ne].c=numero(nc, lista, cont);
            netlist[cont->ne].d=numero(nd, lista, cont);
        }
        else if (tipo=='O') {
            sscanf(p,"%10s%10s%10s%10s",na,nb,nc,nd);
            printf("%s %s %s %s %s\n",netlist[cont->ne].nome,na,nb,nc,nd);
            netlist[cont->ne].a=numero(na, lista, cont);
            netlist[cont->ne].b=numero(nb, lista, cont);
            netlist[cont->ne].c=numero(nc, lista, cont);
            netlist[cont->ne].d=numero(nd, lista, cont);
        }
         else if (tipo=='.') {
            sscanf(p,"%10s%d%lg%lg",ptspor,&freq->npts,&freq->fi,&freq->fs);
            if (strcmp(ptspor,"DEC")==0)
              freq->ptspor=1;
            else if (strcmp(ptspor,"OCT")==0)
              freq->ptspor=2;
            else 
              freq->ptspor=0;
            cont->ne--;
        }
        else if (tipo == 'Q') {
            sscanf(p,"%10s%10s%10s%10s%10s%lg%lg%lg%lg%lg%lg%lg%lg%lg%lg%lg",
                netlist[cont->ne].nome, nc, nb, na,
                netlist[cont->ne].modelo, &netlist[cont->ne].alfa, &netlist[cont->ne].alfar,
                &netlist[cont->ne].isbe,  &netlist[cont->ne].vtbe,  &netlist[cont->ne].isbc, 
                &netlist[cont->ne].vtbc,  &netlist[cont->ne].va,    &netlist[cont->ne].c0be,
                &netlist[cont->ne].c1be,  &netlist[cont->ne].c0bc,  &netlist[cont->ne].c1bc
            );
            // Transistor bipolar: Q<nome> <nóc> <nób> <nóe> <tipo> <α> <αr> <I s be> <V T be> <I s bc> <V T bc> <VA> <C 0 be> <C 1 be> <C 0 bc> <C 1 bc>
            // Q1 1 2 3 NPN 0.995 0.5 1e-9 43.43e-3 1e-9 43.43e-3 100 5e-12 100e-18 5e-12 100e-18
            printf(
                "%s %s %s %s %s %g %g %g %g %g %g %g %g %g %g %g\n",
                netlist[cont->ne].nome, nc, nb, na, 
                netlist[cont->ne].modelo, netlist[cont->ne].alfa, netlist[cont->ne].alfar,
                netlist[cont->ne].isbe,  netlist[cont->ne].vtbe,  netlist[cont->ne].isbc, 
                netlist[cont->ne].vtbc,  netlist[cont->ne].va,    netlist[cont->ne].c0be,
                netlist[cont->ne].c1be,  netlist[cont->ne].c0bc,  netlist[cont->ne].c1bc
            );
            netlist[cont->ne].a=numero(na, lista, cont);
            netlist[cont->ne].b=numero(nb, lista, cont);
            netlist[cont->ne].c=numero(nc, lista, cont);
        }
        else if (tipo=='*') { /* Comentario comeca com "*" */
            printf("Comentario: %s", txt);
            cont->ne--;
        }
        else {
            printf("Elemento desconhecido: %s\n", txt);
            return 1;
        }
    }

    return OK;
}

int variaveisCorrente(contagem *cont, char lista[MAX_NOS+1][MAX_NOME+2], elemento netlist[MAX_ELEM], tabela L, tabela C) {
    /* Acrescenta variaveis de corrente acima dos nos, anotando no netlist */
    char tipo;
    int i;
    cont->nn=cont->nv;
    cont->neq=cont->nn;

    for (i=1; i<=cont->ne; i++) {
        tipo=netlist[i].nome[0];

        if (tipo=='V') {
            cont->nv++;
            if (testarnos(cont)) return 1;
            strcpy(lista[cont->nv],"j"); /* Tem espaco para mais dois caracteres */
            strcat(lista[cont->nv],netlist[i].nome);
            netlist[i].x=cont->nv;
            operacional(netlist[i].a,netlist[i].b,0,netlist[i].x, L, C);
        }
        else if (tipo == 'L') {
            cont->nv++;
            if (testarnos(cont)) return 1;
            strcpy(lista[cont->nv],"j"); /* Tem espaco para mais dois caracteres */
            strcat(lista[cont->nv],netlist[i].nome);
            netlist[i].x=cont->nv;
            operacional(netlist[i].a,netlist[i].b,0,netlist[i].x, L, C);
        }
        else if (tipo=='O') {
            operacional(netlist[i].a,netlist[i].b,netlist[i].c,netlist[i].d, L, C);
            cont->neq--;
        }
        else if (tipo=='E') {
            cont->nv++;
            if (testarnos(cont)) return 1;
            strcpy(lista[cont->nv],"j"); /* Tem espaco para mais dois caracteres */
            strcat(lista[cont->nv],netlist[i].nome);
            netlist[i].x=cont->nv;
            operacional(netlist[i].a,netlist[i].b,0,netlist[i].x, L, C);
        }
        else if (tipo=='F') {
            cont->nv++;
            if (testarnos(cont)) return 1;
            strcpy(lista[cont->nv],"j"); /* Tem espaco para mais dois caracteres */
            strcat(lista[cont->nv],netlist[i].nome);
            netlist[i].x=cont->nv;
            operacional(netlist[i].x,0,netlist[i].c,netlist[i].d, L, C);
        }
        else if (tipo=='H') {
            cont->nv=cont->nv+2;
            if (testarnos(cont)) return 1;
            strcpy(lista[cont->nv-1],"jx"); strcat(lista[cont->nv-1],netlist[i].nome);
            netlist[i].x=cont->nv-1;
            strcpy(lista[cont->nv],"jy"); strcat(lista[cont->nv],netlist[i].nome);
            netlist[i].y=cont->nv;
            operacional(netlist[i].a,netlist[i].b,0,netlist[i].y, L, C);
            operacional(netlist[i].x,0,netlist[i].c,netlist[i].d, L, C);
        }
    }

    return OK;
}

void imprimirNetlist(contagem *cont, char lista[MAX_NOS+1][MAX_NOME+2], elemento netlist[MAX_ELEM], tabela C) {
    char tipo;
    int i;
    
    printf("Variaveis internas: \n");
    for (i=0; i<=cont->nv; i++) {
        printf("%d -> %s (%d)\n",i,lista[i],C[i]);
    }

    printf("Netlist interno final\n");
    for (i=1; i<=cont->ne; i++) {
        tipo=netlist[i].nome[0];

        if (tipo == 'R' || tipo == 'L' || tipo == 'C') {
            printf("%s %d %d %g\n",netlist[i].nome,netlist[i].a,netlist[i].b,netlist[i].valor);
        }
        else if (tipo == 'I' || tipo == 'V') {
            printf("%s %d %d %g %g %g\n",netlist[i].nome,netlist[i].a,netlist[i].b,netlist[i].modulo,netlist[i].fase,netlist[i].valor);
        }
        else if (tipo == 'G' || tipo == 'E' || tipo == 'F' || tipo == 'H') {
            printf("%s %d %d %d %d %g\n",netlist[i].nome,netlist[i].a,netlist[i].b,netlist[i].c,netlist[i].d,netlist[i].valor);
        }
        else if (tipo == 'O') {
            printf("%s %d %d %d %d\n",netlist[i].nome,netlist[i].a,netlist[i].b,netlist[i].c,netlist[i].d);
        }
        else if (tipo == 'Q') {
            printf("%s %d %d %d\n",netlist[i].nome,netlist[i].c,netlist[i].b,netlist[i].a);
        }

        if (tipo == 'V' || tipo == 'E' || tipo == 'F' || tipo == 'O') {
            printf("Corrente jx: %d\n",netlist[i].x);
        }
        else if (tipo == 'H') {
            printf("Correntes jx e jy: %d, %d\n",netlist[i].x,netlist[i].y);
        }
    }

    printf("O circuito tem %d nos, %d variaveis, %d equacoes e %d elementos\n",cont->nn,cont->nv,cont->neq,cont->ne);
}
