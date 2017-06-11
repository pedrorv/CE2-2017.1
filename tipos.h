#ifndef TIPOS_H
#define TIPOS_H

typedef struct elemento { /* Elemento do netlist */
  char nome[MAX_NOME],la[MAX_NOME],lb[MAX_NOME],modelo[MAX_NOME];
  double valor, modulo, fase;
  int a,b,c,d,x,y;
  double alfa, alfar, isbe, vtbe, isbc, vtbc, va, c0be, c1be, c0bc, c1bc;
} elemento;

typedef int tabela[MAX_NOS+1];

typedef struct contagem {
  int
    ne, /* Elementos */
    nv, /* Variaveis */
    neq,/* Equacoes */
    nn; /* Nos */
} contagem;

typedef struct frequencia {
  int
    npts, /* Pontos */
    ptspor; /* 1:por decada, 2:por oitava, 0 ou outros: linear*/
  double
    fi, /* Frequencia de corte inferior */
    fs; /* Frequencia de corte superior */
} frequencia;

#endif