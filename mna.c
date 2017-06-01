#include "tipos.h"
#include "mna.h"
#include <math.h>
#include <complex.h>
#include <stdio.h>

void somar(int *Q, int a, int b) {
  int i,a1,b1;

  if (Q[a]>Q[b]) {a1=Q[b]; b1=Q[a];}
  else {a1=Q[a]; b1=Q[b];}
  if (a1==b1) {printf("Circuito invalido"); exit(1);}
  for (i=1; i<=MAX_NOS; i++) {
    if (Q[i]==b1) Q[i]=a1;
    if (Q[i]>b1) Q[i]--;
  }
}

void operacional (int na,int nb,int nc,int nd, tabela L, tabela C) {
#ifdef DEBUG
  printf("Saida: %d %d; entrada %d %d\n",na,nb,nc,nd);
#endif
  somar(L,na,nb);
  somar(C,nc,nd);
}

void transcondutancia(double gm,int n1,int n2,int n3,int n4, double Yn[MAX_NOS+1][MAX_NOS+2], tabela L, tabela C) {
  Yn[L[n1]][C[n3]]+=gm;
  Yn[L[n2]][C[n4]]+=gm;
  Yn[L[n1]][C[n4]]-=gm;
  Yn[L[n2]][C[n3]]-=gm;
}

void transadmitancia(double _Complex ym,int n1,int n2,int n3,int n4, double _Complex Yn[MAX_NOS+1][MAX_NOS+2], tabela L, tabela C) {
  Yn[L[n1]][C[n3]]+=ym;
  Yn[L[n2]][C[n4]]+=ym;
  Yn[L[n1]][C[n4]]-=ym;
  Yn[L[n2]][C[n3]]-=ym;
}

void condutancia(double g, int a, int b,double Yn[MAX_NOS+1][MAX_NOS+2], tabela L, tabela C){
  transcondutancia(g,a,b,a,b,Yn,L,C);
}

void admitancia(double _Complex  y, int a, int b, double _Complex Yn[MAX_NOS+1][MAX_NOS+2], tabela L, tabela C){
  transadmitancia(y,a,b,a,b,Yn,L,C);
}

void corrente(double i, int a, int b,double  Yn[MAX_NOS+1][MAX_NOS+2], tabela L, tabela C) {
  Yn[L[a]][neq+1]-=i;
  Yn[L[b]][neq+1]+=i;
}

void fasorcorrente(double modulo, double fase, int a, int b, double _Complex Yn[MAX_NOS+1][MAX_NOS+2], tabela L, tabela C) {
  Yn[L[a]][neq+1]-=modulo*(cos(fase)+sin(fase)*I);
  Yn[L[b]][neq+1]+=modulo*(cos(fase)+sin(fase)*I);
}

void mnaPO(elemento netlist[MAX_ELEM],double  Yn[MAX_NOS+1][MAX_NOS+2], double Yn1[MAX_NOS+1][MAX_NOS+2], tabela L, tabela C){
  char tipo;
  int i;
  for (i=1; i<=ne; i++) {
    tipo=netlist[i].nome[0];
    if (tipo=='R') condutancia(1/netlist[i].valor,netlist[i].a,netlist[i].b,Yn1,L,C);
    else if (tipo=='L') condutancia(GMAX,netlist[i].a,netlist[i].b,Yn1,L,C);
    else if (tipo=='C') condutancia(0,netlist[i].a,netlist[i].b,Yn1,L,C);
    else if (tipo=='G') transcondutancia(netlist[i].valor,netlist[i].a,netlist[i].b,netlist[i].c,netlist[i].d,Yn1,L,C);
    else if (tipo=='K');
    else if (tipo=='I') corrente(netlist[i].valor,netlist[i].a,netlist[i].b,Yn1,L,C);
    else if (tipo=='V') {
      transcondutancia(1,0,netlist[i].x,netlist[i].a,netlist[i].b,Yn1,L,C);
      corrente(netlist[i].valor,netlist[i].x,0,Yn1,L,C);
    }
    else if (tipo=='E') {
      transcondutancia(1,0,netlist[i].x,netlist[i].a,netlist[i].b,Yn1,L,C);
      transcondutancia(netlist[i].valor,netlist[i].x,0,netlist[i].c,netlist[i].d,Yn1,L,C);
    }
    else if (tipo=='F') {
      transcondutancia(netlist[i].valor,netlist[i].a,netlist[i].b,netlist[i].x,0,Yn1,L,C);
      transcondutancia(1,netlist[i].c,netlist[i].d,netlist[i].x,0,Yn1,L,C);
    }
    else if (tipo=='H') {
      transcondutancia(1,0,netlist[i].y,netlist[i].a,netlist[i].b,Yn1,L,C);
      transcondutancia(netlist[i].valor,netlist[i].y,0,netlist[i].x,0,Yn1,L,C);
      transcondutancia(1,netlist[i].c,netlist[i].d,netlist[i].x,0,Yn1,L,C);
    }
    else if (tipo=='O') {
    }
}

void mnaPS(elemento netlist[MAX_ELEM], double _Complex Yn[MAX_NOS+1][MAX_NOS+2], tabela L, tabela C, double f){
  char tipo;
  int i;
  double _Complex jw;
  
  jw=2*PI*f*I;
    
  for (i=1; i<=ne; i++) {
    tipo=netlist[i].nome[0];
    if (tipo=='R') admitancia(1/netlist[i].valor,netlist[i].a,netlist[i].b,Yn,L,C);
    else if (tipo=='L') admitancia(1/(netlist[i].valor*jw),netlist[i].a,netlist[i].b,Yn,L,C);
    else if (tipo=='C') admitancia((netlist[i].valor*jw),netlist[i].a,netlist[i].b,Yn,L,C);
    else if (tipo=='G') admitancia(netlist[i].valor,netlist[i].a,netlist[i].b,netlist[i].c,netlist[i].d,Yn,L,C);
    else if (tipo=='K');
    else if (tipo=='I') fasorcorrente(netlist[i].modulo,netlist[i].fase,netlist[i].a,netlist[i].b,Yn,L,C);
    else if (tipo=='V') {
      transadmitancia(1,0,netlist[i].x,netlist[i].a,netlist[i].b,Yn,L,C);
      fasorcorrente(netlist[i].modulo,netlist[i].fase,netlist[i].x,0,Yn,L,C);
    }
    else if (tipo=='E') {
      transadmitancia(1,0,netlist[i].x,netlist[i].a,netlist[i].b,Yn,L,C);
      transadmitancia(netlist[i].valor,netlist[i].x,0,netlist[i].c,netlist[i].d,Yn,L,C);
    }
    else if (tipo=='F') {
      transadmitancia(netlist[i].valor,netlist[i].a,netlist[i].b,netlist[i].x,0,Yn,L,C);
      transadmitancia(1,netlist[i].c,netlist[i].d,netlist[i].x,0,Yn,L,C);
    }
    else if (tipo=='H') {
      transadmitancia(1,0,netlist[i].y,netlist[i].a,netlist[i].b,Yn,L,C);
      transadmitancia(netlist[i].valor,netlist[i].y,0,netlist[i].x,0,Yn,L,C);
      transadmitancia(1,netlist[i].c,netlist[i].d,netlist[i].x,0,Yn,L,C);
    }
    else if (tipo=='O') {
    }
}
