#ifndef TIPOS_H
#define TIPOS_H

typedef struct elemento { /* Elemento do netlist */
  char nome[MAX_NOME],la[MAX_NOME],lb[MAX_NOME];
  double valor, modulo, fase;
  int a,b,c,d,x,y;
} elemento;

typedef int tabela[MAX_NOS+1];

typedef struct contagem {
  int
    ne, /* Elementos */
    nv, /* Variaveis */
    nn; /* Nos */
} contagem;

#endif