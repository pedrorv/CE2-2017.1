#include "tipos.h"
#include "mna.h"
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

void transcondutancia(double gm,int n1,int n2,int n3,int n4, Yn[MAX_NOS+1][MAX_NOS+2], tabela L, tabela C) {
  Yn[L[n1]][C[n3]]+=gm;
  Yn[L[n2]][C[n4]]+=gm;
  Yn[L[n1]][C[n4]]-=gm;
  Yn[L[n2]][C[n3]]-=gm;
}

void condutancia(double g, int a, int b, Yn[MAX_NOS+1][MAX_NOS+2], tabela L, tabela C){
  transcondutancia(g,a,b,a,b,Yn,L,C);
}

void corrente(double i, int a, int b, Yn[MAX_NOS+1][MAX_NOS+2], tabela L, tabela C) {
  Yn[L[a]][neq+1]-=i;
  Yn[L[b]][neq+1]+=i;
}
