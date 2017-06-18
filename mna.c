#include <complex.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "macros.h"
#include "mna.h"
#include "tipos.h"
#include "resolver.h"

void somar(int *Q, int a, int b) {
  int i,a1,b1;

  if (Q[a]>Q[b]) {
    a1=Q[b]; b1=Q[a];
  }
  else {
    a1=Q[a]; b1=Q[b];
  }

  if (a1==b1) {
    printf("Circuito invalido"); 
    exit(1);
  }

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

void corrente(double i, int a, int b,double  Yn[MAX_NOS+1][MAX_NOS+2], tabela L, tabela C, contagem *cont) {
  Yn[L[a]][cont->neq+1]-=i;
  Yn[L[b]][cont->neq+1]+=i;
}

void fasorcorrente(double modulo, double fase, int a, int b, double _Complex Yn[MAX_NOS+1][MAX_NOS+2], tabela L, tabela C, contagem *cont) {
  Yn[L[a]][cont->neq+1]-=modulo*(cos(fase)+sin(fase)*I);
  Yn[L[b]][cont->neq+1]+=modulo*(cos(fase)+sin(fase)*I);
}

void acoplamento(double k,char la[],char lb[],elemento netlist[MAX_ELEM],double _Complex Yn[MAX_NOS+1][MAX_NOS+2], tabela L, tabela C, double f, contagem *cont){
  int i,a,b;
  double l1,l2,m;
  double _Complex jw=2*M_PI*f*I;
  
  for (i=1; i<=cont->ne; i++) {
    if(netlist[i].nome==la){
      l1=netlist[i].valor;
      a=i;
    }
    if(netlist[i].nome==lb){
      l2=netlist[i].valor;
      b=i;
    }
  }

  m=k*sqrt(l1*l2);
  transadmitancia(m*jw,netlist[a].x,0,netlist[b].x,0,Yn,L,C);
  transadmitancia(m*jw,netlist[b].x,0,netlist[a].x,0,Yn,L,C);
}

void mnaPO(elemento netlist[MAX_ELEM], double YnPO[MAX_NOS+1][MAX_NOS+2], double Yn[MAX_NOS+1][MAX_NOS+2], tabela L, tabela C, contagem *cont){
  char tipo;
  int i;
  double VBE,GBE,IBE,VBC,GBC,IBC;

  for (i=1; i<=cont->ne; i++) {
    tipo=netlist[i].nome[0];

    if (tipo=='R') {
      condutancia(1/netlist[i].valor,netlist[i].a,netlist[i].b,Yn,L,C);
    }
    else if (tipo=='L') {
      transcondutancia(1,0,netlist[i].x,netlist[i].a,netlist[i].b,Yn,L,C);
      condutancia(GMIN,netlist[i].x,0,Yn,L,C);
    }
    else if (tipo=='C') {
      condutancia(GMIN,netlist[i].a,netlist[i].b,Yn,L,C);
    }
    else if (tipo=='G') {
      transcondutancia(netlist[i].valor,netlist[i].a,netlist[i].b,netlist[i].c,netlist[i].d,Yn,L,C);
    }
    else if (tipo=='K');
    else if (tipo=='I') {
      corrente(netlist[i].valor,netlist[i].a,netlist[i].b,Yn,L,C,cont);
    }
    else if (tipo=='V') {
      transcondutancia(1,0,netlist[i].x,netlist[i].a,netlist[i].b,Yn,L,C);
      corrente(netlist[i].valor,netlist[i].x,0,Yn,L,C,cont);
    }
    else if (tipo=='E') {
      transcondutancia(1,0,netlist[i].x,netlist[i].a,netlist[i].b,Yn,L,C);
      transcondutancia(netlist[i].valor,netlist[i].x,0,netlist[i].c,netlist[i].d,Yn,L,C);
    }
    else if (tipo=='F') {
      transcondutancia(netlist[i].valor,netlist[i].a,netlist[i].b,netlist[i].x,0,Yn,L,C);
      transcondutancia(1,netlist[i].c,netlist[i].d,netlist[i].x,0,Yn,L,C);
    }
    else if (tipo=='H') {
      transcondutancia(1,0,netlist[i].y,netlist[i].a,netlist[i].b,Yn,L,C);
      transcondutancia(netlist[i].valor,netlist[i].y,0,netlist[i].x,0,Yn,L,C);
      transcondutancia(1,netlist[i].c,netlist[i].d,netlist[i].x,0,Yn,L,C);
    }
    else if (tipo=='O');
    else if (tipo=='Q') {
      VBE=YnPO[L[netlist[i].b]][C[cont->neq+1]]-YnPO[L[netlist[i].a]][C[cont->neq+1]];
      //if (VBE>0.8) VBE=0.8; 
      GBE=netlist[i].isbe*exp(VBE/netlist[i].vtbe)/netlist[i].vtbe;
      IBE=netlist[i].isbe*(exp(VBE/netlist[i].vtbe)-1) - GBE*VBE;
      VBC=YnPO[L[netlist[i].b]][C[cont->neq+1]]-YnPO[L[netlist[i].c]][C[cont->neq+1]];
      //if (VBC>0.8) VBC=0.8;
      GBC=netlist[i].isbc*exp(VBC/netlist[i].vtbc)/netlist[i].vtbc;
      IBC=netlist[i].isbc*(exp(VBC/netlist[i].vtbc)-1) - GBE*VBC;

      if (netlist[i].modelo[0] == 'N') {
        condutancia(GBE,netlist[i].b,netlist[i].a,Yn,L,C);
        corrente(IBE,netlist[i].b,netlist[i].a,Yn,L,C,cont);
        corrente(netlist[i].alfar*IBC,netlist[i].a,netlist[i].b,Yn,L,C,cont);
        transcondutancia(netlist[i].alfar*GBC,netlist[i].a,netlist[i].b,netlist[i].b,netlist[i].c,Yn,L,C);
        condutancia(GBC,netlist[i].b,netlist[i].c,Yn,L,C);
        corrente(IBC,netlist[i].b,netlist[i].c,Yn,L,C,cont);
        corrente(netlist[i].alfa*IBE,netlist[i].c,netlist[i].b,Yn,L,C,cont);
        transcondutancia(netlist[i].alfa*GBE,netlist[i].c,netlist[i].b,netlist[i].b,netlist[i].a,Yn,L,C);
      }
      else if (netlist[i].modelo[0] == 'P') {
        condutancia(GBE,netlist[i].b,netlist[i].a,Yn,L,C);
        corrente(-IBE,netlist[i].b,netlist[i].a,Yn,L,C,cont);
        corrente(-netlist[i].alfar*IBC,netlist[i].a,netlist[i].b,Yn,L,C,cont);
        transcondutancia(-netlist[i].alfar*GBC,netlist[i].a,netlist[i].b,netlist[i].b,netlist[i].c,Yn,L,C);
        condutancia(GBC,netlist[i].b,netlist[i].c,Yn,L,C);
        corrente(-IBC,netlist[i].b,netlist[i].c,Yn,L,C,cont);
        corrente(-netlist[i].alfa*IBE,netlist[i].c,netlist[i].b,Yn,L,C,cont);
        transcondutancia(-netlist[i].alfa*GBE,netlist[i].c,netlist[i].b,netlist[i].b,netlist[i].a,Yn,L,C);
      }
    }

    printf("Sistema apos a estampa de %s\n", netlist[i].nome);
    imprimeSistemaDouble(Yn, cont);
  }
}

void mnaPS(elemento netlist[MAX_ELEM], double YnPO[MAX_NOS+1][MAX_NOS+2], double _Complex Yn[MAX_NOS+1][MAX_NOS+2], tabela L, tabela C, double f, contagem *cont) {
  char tipo;
  int i;
  double _Complex jw=2*M_PI*f*I;
  double VBE,GBE,VBC,GBC;
   
  for (i=1; i<=cont->ne; i++) {
    tipo=netlist[i].nome[0];

    if (tipo=='R') {
      admitancia(1/netlist[i].valor,netlist[i].a,netlist[i].b,Yn,L,C);
    }
    else if (tipo=='L') {
      transadmitancia(1,0,netlist[i].x,netlist[i].a,netlist[i].b,Yn,L,C);
      admitancia((netlist[i].valor*jw),netlist[i].x,0,Yn,L,C);
    }
    else if (tipo=='C') {
      admitancia((netlist[i].valor*jw),netlist[i].a,netlist[i].b,Yn,L,C);
    }
    else if (tipo=='G') {
      transadmitancia(netlist[i].valor,netlist[i].a,netlist[i].b,netlist[i].c,netlist[i].d,Yn,L,C);
    }
    else if (tipo=='K') {
      acoplamento(netlist[i].valor,netlist[i].la,netlist[i].lb,netlist,Yn,L,C,f,cont);
    }
    else if (tipo=='I') {
      fasorcorrente(netlist[i].modulo,netlist[i].fase,netlist[i].a,netlist[i].b,Yn,L,C,cont);
    }
    else if (tipo=='V') {
      transadmitancia(1,0,netlist[i].x,netlist[i].a,netlist[i].b,Yn,L,C);
      fasorcorrente(netlist[i].modulo,netlist[i].fase,netlist[i].x,0,Yn,L,C,cont);
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
    else if (tipo=='Q') {
      VBE = YnPO[L[netlist[i].b]][C[cont->neq+1]] - YnPO[L[netlist[i].a]][C[cont->neq+1]];
      //if (VBE>0.8) VBE=0.8; 
      GBE = netlist[i].isbe * exp(VBE/netlist[i].vtbe) / netlist[i].vtbe;
      VBC = YnPO[L[netlist[i].b]][C[cont->neq+1]] - YnPO[L[netlist[i].c]][C[cont->neq+1]];
      //if (VBC>0.8) VBC=0.8;
      GBC = netlist[i].isbc * exp(VBC/netlist[i].vtbc) / netlist[i].vtbc;

      if (netlist[i].modelo[0] == 'N') {
        admitancia(GBE, netlist[i].b, netlist[i].a, Yn, L, C);
        if (VBE>0.3)
          admitancia(((netlist[i].c0be / sqrt(1 - VBE/0.6)) * jw), netlist[i].b, netlist[i].a, Yn, L, C); 
        else 
          admitancia(((netlist[i].c0be / sqrt(0.5)) * jw), netlist[i].b, netlist[i].a, Yn, L, C);  /* Creversa BE */
        admitancia(((netlist[i].c1be * (exp(VBE/netlist[i].vtbe) - 1)) * jw), netlist[i].b, netlist[i].a, Yn, L, C); /* Cdireta BE */
        transadmitancia(netlist[i].alfar*GBC, netlist[i].a, netlist[i].b, netlist[i].b, netlist[i].c, Yn, L, C);
        admitancia(GBC, netlist[i].b, netlist[i].c, Yn, L, C);
        if (VBC>0.3)
          admitancia(((netlist[i].c0bc / sqrt(1 - VBC/0.6)) * jw), netlist[i].b, netlist[i].c, Yn, L, C);
        else
          admitancia(((netlist[i].c0bc / sqrt(0.5)) * jw), netlist[i].b, netlist[i].c, Yn, L, C); /* Creversa BC */
        admitancia(((netlist[i].c1bc * (exp(VBC/netlist[i].vtbc) - 1)) * jw), netlist[i].b, netlist[i].c, Yn, L, C); /* Cdireta BC */
        transadmitancia(netlist[i].alfa*GBE, netlist[i].c, netlist[i].b, netlist[i].b, netlist[i].a, Yn, L, C);
      }
      else if (netlist[i].modelo[0] == 'P') {
        admitancia(GBE, netlist[i].b, netlist[i].a, Yn, L, C);
        if (VBE>0.3)
          admitancia(((netlist[i].c0be / sqrt(1 - VBE/0.6)) * jw), netlist[i].b, netlist[i].a, Yn, L, C); 
        else 
          admitancia(((netlist[i].c0be / sqrt(0.5)) * jw), netlist[i].b, netlist[i].a, Yn, L, C);  /* Creversa BE */
        admitancia(((netlist[i].c1be * (exp(VBE/netlist[i].vtbe) - 1)) * jw), netlist[i].b, netlist[i].a, Yn, L, C); /* Cdireta BE */
        transadmitancia(-netlist[i].alfar*GBC, netlist[i].a, netlist[i].b, netlist[i].b, netlist[i].c, Yn, L, C);
        admitancia(GBC, netlist[i].b, netlist[i].c, Yn, L, C);
        if (VBC>0.3)
          admitancia(((netlist[i].c0bc / sqrt(1 - VBC/0.6)) * jw), netlist[i].b, netlist[i].c, Yn, L, C);
        else
          admitancia(((netlist[i].c0bc / sqrt(0.5)) * jw), netlist[i].b, netlist[i].c, Yn, L, C); /* Creversa BC */
        admitancia(((netlist[i].c1bc * (exp(VBC/netlist[i].vtbc) - 1)) * jw), netlist[i].b, netlist[i].c, Yn, L, C); /* Cdireta BC */
        transadmitancia(-netlist[i].alfa*GBE, netlist[i].c, netlist[i].b, netlist[i].b, netlist[i].a, Yn, L, C);
      }
    }  
    else if (tipo=='O');
  }
}
